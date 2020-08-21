function getval(x) {
    d = document.getElementById(x).value;
    if (x === "sj")
    {
        document.getElementById('sjW').innerHTML = d;
        return 0;
    }
    else if (x === "jae")
    {
        document.getElementById('jaeW').innerHTML = d;
        return 0;
    }
    else if (x === "yk")
    {
        document.getElementById('ykW').innerHTML = d;
        return 0;
    }
    else if (x === "w")
    {
        document.getElementById('wW').innerHTML = d;
        return 0;
    }
    else if (x === "dw")
    {
        document.getElementById('dwW').innerHTML = d;
        return 0;
    }
}

