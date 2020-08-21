function getval(x) {
    d = document.getElementById(x).value;
    if (x === "rm")
    {
        document.getElementById('rmW').innerHTML = d;
        return 0;
    }
    else if (x === "jin")
    {
        document.getElementById('jinW').innerHTML = d;
        return 0;
    }
    else if (x === "suga")
    {
        document.getElementById('sugaW').innerHTML = d;
        return 0;
    }
    else if (x === "jh")
    {
        document.getElementById('jhW').innerHTML = d;
        return 0;
    }
    else if (x === "jm")
    {
        document.getElementById('jmW').innerHTML = d;
        return 0;
    }
    else if (x === "v")
    {
        document.getElementById('vW').innerHTML = d;
        return 0;
    }
    else if (x === "jk")
    {
        document.getElementById('jkW').innerHTML = d;
        return 0;
    }
}

