import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required

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

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///project.db")


@app.route("/home", methods=["GET", "POST"])
@login_required
def home():
    # Create tables if they don't exist TASKS TRACKER
    db.execute("CREATE TABLE IF NOT EXISTS projects (project_id INTEGER NOT NULL, user_pro INTEGER NOT NULL, name TEXT NOT NULL, num_tasks NUMERIC DEFAULT 0, FOREIGN KEY(user_pro) REFERENCES users(id), PRIMARY KEY(project_id))")

    if request.method == "POST":
        #get the text that was written
        if not request.form.get("projectname"):
            return apology("must provide the name of your project", 403)

        projectname = request.form.get("projectname")
        #add project name to users table
        db.execute("INSERT INTO projects (name, user_pro) VALUES (?, ?)", projectname, session["user_id"])
        # flash message that the name was registered
        flash("Registration of project name successful")

    # count number of tasks of each project
    k = db.execute("SELECT project_id FROM projects WHERE user_pro = ?", session["user_id"])
    for i in range(len(k)):
        num = db.execute("SELECT task FROM tasks WHERE (task_id = ? AND user_task = ?)", k[i]["project_id"], session["user_id"])
        db.execute("UPDATE projects SET num_tasks = ? WHERE (project_id = ? AND user_pro = ?)", len(num), k[i]["project_id"], session["user_id"])

    # Display projects in a list with number of tasks left to do
    list_proj = []
    projects = db.execute("SELECT name, num_tasks FROM projects WHERE user_pro = ?", session["user_id"])
    for i in range(len(projects)):
        proj = [projects[i]["name"], projects[i]["num_tasks"]]
        list_proj.append(proj)

    # Renders the home html
    return render_template("home.html", list_proj=list_proj, num_proj=len(projects))


@app.route("/tasks", methods=["GET", "POST"])
@login_required
def tasks():
    # create table for tasks
    db.execute("CREATE TABLE IF NOT EXISTS tasks (task_id INTEGER NOT NULL, user_task INTEGER NOT NULL, task TEXT NOT NULL, due_date DATE NOT NULL, priority NUMERIC NOT NULL, status NUMERIC DEFAULT 0, FOREIGN KEY(task_id) REFERENCES projects(project_id))")

    if request.method == "POST" and ("insert" in request.form or "update" in request.form): # this condition if for only two buttons, to insert and update a task
        # add tasks to database
        if not request.form.get("task") or not request.form.get("date") or not request.form.get("priority") or not request.form.get("project"):
            return apology("must fill in the three spaces", 403) # if the user doesn't fill out or choose something

        # get the input of the user's task
        name = request.form.get("project")
        project = db.execute("SELECT project_id FROM projects WHERE name = ?", name)
        task = request.form.get("task")
        date = request.form.get("date")
        priority = request.form.get("priority")

        # different conditions to determine what button was clicked
        if "insert" in request.form: # condition if they clicked the button to add a task
            #add project name to users table
            db.execute("INSERT INTO tasks (task_id, user_task, task, due_date, priority) VALUES (?, ?, ?, ?, ?)", project[0]["project_id"], session["user_id"], task, date, priority)
            # flash message that the name was registered
            flash("Task successfully saved")
        elif "update" in request.form: # condition if they clicked the button to update the task with the new edits
            # get the old task name so I know what task to update
            old_task = request.form.get("old task")
            # update the task in the database
            db.execute("UPDATE tasks SET task_id=?, user_task=?, task=?, due_date=?, priority=? WHERE task=?", project[0]["project_id"], session["user_id"], task, date, priority, old_task)
            #flash message that the task was updated successfully
            flash("Task successfully updated")

    elif request.method == "POST" and "delete" in request.form: # this condition is for the button to delete a task
        # first get the task they want to delete
        writtentask=request.form.get("writtentask")
        # delete the task from the database using the task as a condition
        db.execute("DELETE FROM tasks WHERE task=?", writtentask)

    elif request.method == "POST" and "edit" in request.form: # this condition is for the button to edit a task
        # make a list so I can store the projects they have, so I can give them the option to change a task to another project
        projects = []
        # get the projects of the current user from the database
        list_projects = db.execute("SELECT name, project_id FROM projects WHERE user_pro = ?", session["user_id"])
        # use a for loop to build the list of projects
        for j in range(len(list_projects)):
            ongoing = [list_projects[j]["name"], list_projects[j]["project_id"]]
            projects.append(ongoing)
        writtentask = request.form.get("writtentask") # get the task they want to edit
        # get all the values of the task they wish to edit, so I can display them on the html
        values = db.execute("SELECT task_id, task, due_date, priority FROM tasks WHERE task = ?", writtentask)
        # get the name of the project the task is from
        project = db.execute("SELECT name FROM projects WHERE project_id = ?", values[0]["task_id"])
        # render another html, the edit.html so they can update the task
        return render_template("edit.html", project=project[0]["name"], task=values[0]["task"], date=values[0]["due_date"], priority=values[0]["priority"], projects=projects)

    elif request.method == "POST" and "not done" in request.form:
        # this condition is for when a user clicks the "checkbox" button, so the database stores the new value, 1, which means the task is done
        db.execute("UPDATE tasks SET status = ? WHERE task = ?", 1, request.form.get("writtentask"))
    elif request.method == "POST" and "done" in request.form:
        # this condition is for when a user clicks the "checkbox" button, so the database stores the new value, 0, which means the task is undone
        db.execute("UPDATE tasks SET status = ? WHERE task = ?", 0, request.form.get("writtentask"))
    elif request.method == "POST" and "filter" in request.form:
        # this condition is for the select option to filter the tasks and see different views of them
        fil_project = request.form.get("option") # get the option they choose

        if fil_project != "All Tasks" and fil_project != "TU" and fil_project != "TC": # this condition applies when the user chooses a project as a filter
            # display tasks of the user
            tasks = [] # empty list for the tasks belonging to a certain project
            projects = [] # empty list for the users projects, so they can appear in the select options
            list_tasks = db.execute("SELECT task, due_date, priority, status FROM tasks WHERE (user_task = ? AND task_id = ?) ORDER BY due_date ASC, priority ASC", session["user_id"], fil_project)
            list_projects = db.execute("SELECT name, project_id FROM projects WHERE user_pro = ?", session["user_id"])
            for i in range(len(list_tasks)): # get the data (tasks) from the database into the empty list
                todo = [list_tasks[i]["task"], list_tasks[i]["due_date"], list_tasks[i]["priority"], list_tasks[i]["status"]]
                tasks.append(todo)
            for j in range(len(list_projects)): # get the data (projects) from the database into the empty list
                ongoing = [list_projects[j]["name"], list_projects[j]["project_id"]]
                projects.append(ongoing)
            return render_template("tasks.html", tasks=tasks, projects=projects, fil_project=fil_project)

        elif fil_project == "TC": # this condition is for when the user wants to see the completed tasks only
            # display tasks of the user
            tasks = [] # empty list for the tasks that have been completed
            projects = [] # empty list for the users projects, so they can appear in the select options
            list_tasks = db.execute("SELECT task, due_date, priority, status FROM tasks WHERE (user_task = ? AND status = ?) ORDER BY due_date ASC, priority ASC", session["user_id"], 1)
            list_projects = db.execute("SELECT name, project_id FROM projects WHERE user_pro = ?", session["user_id"])
            for i in range(len(list_tasks)): # gets the tasks from the current user with the status 1 (done) from the table into the empty list
                todo = [list_tasks[i]["task"], list_tasks[i]["due_date"], list_tasks[i]["priority"], list_tasks[i]["status"]]
                tasks.append(todo)
            for j in range(len(list_projects)): # gets the projects from the user into an empty list
                ongoing = [list_projects[j]["name"], list_projects[j]["project_id"]]
                projects.append(ongoing)
            return render_template("tasks.html", tasks=tasks, projects=projects, fil_project=fil_project)

        elif fil_project == "TU": # this condition is for when the user wants to see the unfinished tasks only
            # display tasks of the user
            tasks = [] # empty list for the tasks that have been completed
            projects = [] # empty list for the users projects, so they can appear in the select options
            list_tasks = db.execute("SELECT task, due_date, priority, status FROM tasks WHERE (user_task = ? AND status = ?) ORDER BY due_date ASC, priority ASC", session["user_id"], 0)
            list_projects = db.execute("SELECT name, project_id FROM projects WHERE user_pro = ?", session["user_id"])
            for i in range(len(list_tasks)): # gets the tasks from the current user with the status 0 (not done) from the table into the empty list
                todo = [list_tasks[i]["task"], list_tasks[i]["due_date"], list_tasks[i]["priority"], list_tasks[i]["status"]]
                tasks.append(todo)
            for j in range(len(list_projects)): # gets the projects from the user into an empty list
                ongoing = [list_projects[j]["name"], list_projects[j]["project_id"]]
                projects.append(ongoing)
            return render_template("tasks.html", tasks=tasks, projects=projects, fil_project=fil_project)

    # display tasks of the user
    tasks = [] # empty list for the tasks of the current user
    projects = [] # empty list for the user's projects
    list_tasks = db.execute("SELECT task, due_date, priority, status FROM tasks WHERE user_task = ? ORDER BY due_date ASC, priority ASC", session["user_id"])
    list_projects = db.execute("SELECT name, project_id FROM projects WHERE user_pro = ?", session["user_id"])
    for i in range(len(list_tasks)): # gets the tasks from the database into the empty list
        todo = [list_tasks[i]["task"], list_tasks[i]["due_date"], list_tasks[i]["priority"], list_tasks[i]["status"]]
        tasks.append(todo)
    for j in range(len(list_projects)): # gets the projects into the empty list
        ongoing = [list_projects[j]["name"], list_projects[j]["project_id"]]
        projects.append(ongoing)

    return render_template("tasks.html", tasks=tasks, projects=projects)


@app.route("/")
def start(): # this is for when someone first enters the website, so they're introduced to the webpage
    return render_template("start.html")


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
        return redirect("/home")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login1.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # Make users table if doesn't exist
    db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER NOT NULL, username TEXT NOT NULL, hash TEXT NOT NULL, projectname TEXT, PRIMARY KEY(id))")

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
        return redirect("/home")
    else:
        """send to register html"""
        return render_template("register1.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
