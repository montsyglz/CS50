import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Create current and history
    db.execute("CREATE TABLE IF NOT EXISTS current (new_id INTEGER NOT NULL, symbol TEXT NOT NULL, name TEXT NOT NULL, shares INTEGER NOT NULL, price NUMERIC NOT NULL, total NUMERIC NOT NULL, FOREIGN KEY(new_id) REFERENCES users(id), PRIMARY KEY(symbol,new_id))")
    db.execute("CREATE TABLE IF NOT EXISTS history (his_id INTEGER NOT NULL, symbol TEXT NOT NULL, shares INTEGER NOT NULL, price NUMERIC NOT NULL, time TEXT NOT NULL, FOREIGN KEY(his_id) REFERENCES users(id))")

    cash = 0
    rows = []
    shares = db.execute("SELECT symbol, shares FROM current WHERE new_id = ? ORDER BY symbol", session["user_id"])

    for row in shares:
        looked = lookup(row["symbol"])
        per_symbol = [looked["symbol"], looked["name"], row["shares"], looked["price"], "{:.2f}".format(looked["price"]*float(row["shares"]))]
        cash += looked["price"]*float(row["shares"])
        rows.append(per_symbol)
    cash_left = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash_total = cash + cash_left[0]["cash"]

    return render_template("index.html", rows=rows, cash_left="{:.2f}".format(cash_left[0]["cash"]), cash_total="{:.2f}".format(cash_total))

    #return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        looked = lookup(request.form.get("symbol"))
        shares = request.form.get("shares")

        # send apologies if invalid input
        if looked == None:
            return apology("that symbol doesn't exist", 403)
        elif not shares.isdigit() or int(shares) < 1:
            return apology("shares must be a positive integer", 403)

        # check if the user has enough money to buy the shares and update cash
        cash_user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash_pay = float(shares) * looked["price"]
        if cash_pay > cash_user[0]["cash"]:
            apology("cannot afford the number of shares at the current price", 403)
        cash_new = cash_user[0]["cash"] - cash_pay
        # update cash in users table
        db.execute("UPDATE users SET cash=? WHERE id = ?", cash_new, session["user_id"])

        # make new current tables if not exist and update
        try:
            db.execute("INSERT INTO current (symbol, name, shares, price, total, new_id) VALUES (?, ?, ?, ?, ?, ?)", looked["symbol"], looked["name"], shares, looked["price"], cash_pay, session["user_id"])
        except:
            db.execute("UPDATE current SET shares = shares + ?, total = total + ?, price = ? WHERE new_id = ? AND symbol = ?", shares, cash_pay, looked["price"], session["user_id"], looked["symbol"])
            # maybe update price?

        # make new history table is not exists and update
        db.execute("INSERT INTO history (symbol, shares, price, time, his_id) VALUES (?, ?, ?, datetime(), ?)", looked["symbol"], shares, looked["price"], session["user_id"])

        # redirect to home screen
        flash("Bought!")
        return redirect("/")

        #return apology("TODO")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = []
    values = db.execute("SELECT symbol, shares, price, time FROM history WHERE his_id = ?", session["user_id"])

    for i in range(len(values)):
        per_symbol = [values[i]["symbol"], values[i]["shares"], values[i]["price"], values[i]["time"]]
        rows.append(per_symbol)

    return render_template("history.html", rows=rows)
    #return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # returns dict with 'name' of comp, 'price' of share float, symbol
        looked = lookup(request.form.get("symbol"))
        if looked == None:
            return apology("that symbol doesn't exist", 403)
        return render_template("quoted.html", name=looked["name"], price="{:.2f}".format(looked["price"]), sym=looked["symbol"])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    if request.method == "POST":
        """let's see"""
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password again was submitted
        elif not request.form.get("password_again"):
            return apology("must provide password", 403)

        # Ensure password again is equal to password
        elif not request.form.get("password") == request.form.get("password_again"):
            return apology("must provide the same password", 403)

        elif len(request.form.get("password")) < 8:
            return apology("password must be at least 8 characters", 403)

        # Variables for username and hash number
        user = request.form.get("username")
        hash_num = generate_password_hash(request.form.get("password"))

        # Confirm if the username exists
        confirm = db.execute("SELECT * FROM users WHERE username = ?", user)

        # Ensure username exists and password is correct
        """all system stored procedures return a value of 0. This indicates success and a nonzero value indicates failure"""
        if confirm != []:
            return apology("that username is taken", 403)

        # Query database to store user's info
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", user, hash_num)
        rows = db.execute("SELECT * FROM users WHERE username = ?", user)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Registration successful")
        return redirect("/")
    else:
        """send to register html"""
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        looked = lookup(request.form.get("symbol"))
        shares = request.form.get("shares")

        # send apologies if invalid input
        if looked == None:
            return apology("that symbol doesn't exist", 403)
        elif not shares.isdigit() or int(shares) < 1:
            return apology("shares must be a positive integer", 403)

        # update history table
        db.execute("INSERT INTO history VALUES (?, ?, ?, ?, datetime())", session["user_id"], looked["symbol"], 0-int(shares), looked["price"])

        # update current table
        db.execute("UPDATE current SET shares = shares - ? WHERE new_id = ? AND symbol = ?", shares, session["user_id"], looked["symbol"])
        db.execute("DELETE FROM current WHERE shares = 0")

        # update user cash table
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", looked["price"]*float(shares), session["user_id"])

        flash("Sold!")
        return redirect("/")
    else:
        row=[]
        sym = db.execute("SELECT symbol FROM current WHERE new_id = ?", session["user_id"])
        print(sym)
        for i in range(len(sym)):
            row.append(sym[i]["symbol"])
        print(row)
        return render_template("sell.html", row=row)

    #return apology("TODO")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
