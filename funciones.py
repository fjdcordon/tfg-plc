import sqlite3
import math
import datetime

def round_up(n, decimals=0):
    multiplier = 10 ** decimals
    return math.ceil(n * multiplier) / multiplier

def abrirConex():
    conn = sqlite3.connect("/root/db.sqlite3")
    return conn

def getAnguloOpt():
    conn = abrirConex()
    cur = conn.cursor()
    cur.execute("SELECT * FROM angulos WHERE mes=?", (datetime.datetime.now().month,))
    r = cur.fetchall()
    return r[0][1]

def moverPlaca(angulo):
    try:
        conn = abrirConex()
        cur = conn.cursor()
        sql = "INSERT INTO registroPlaca(timestamp,angulo) VALUES(?,?)"
        cur.execute(sql, (int(datetime.datetime.now().timestamp()), angulo))
        conn.commit()
        conn.close()
        return 0
    except:
        return -1

def calcularEnergia():

    try:
        status = getStatus()
    except:
        return -1

    if status == 0:
        return 0

    else:
        POTENCIA = 500

        H_ACTUAL = datetime.datetime.now().hour

        try:
            A_OPTIMO = getAnguloOpt()
        except:
            return -1

        try:
            A_ACTUAL = getAnguloAct()

        except:
            A_ACTUAL = A_OPTIMO


        try:
            conn = abrirConex()
            cur = conn.cursor()

            cur.execute("SELECT * FROM horasSol WHERE mes=?", (datetime.datetime.now().month,))
            r = cur.fetchall()
            H_SALIDA_SOL = r[0][1]
            H_PUESTA_SOL = r[0][2]

            MAX_SOL = (3 / 4) * POTENCIA
            MAX_ANG = (1 / 4) * POTENCIA
            H_PUNTA = int(round_up((H_SALIDA_SOL + H_PUESTA_SOL) / 2))
            H_DETRAS = H_PUNTA - H_SALIDA_SOL
            H_DELANTE = H_PUESTA_SOL - H_PUNTA
            DIV_DETRAS = MAX_SOL / (H_DETRAS + 1)
            DIV_DELANTE = MAX_SOL / (H_DELANTE + 1)
            ENERGIA: float = 0.000

            if H_SALIDA_SOL <= H_ACTUAL <= H_PUESTA_SOL:
                if H_ACTUAL > H_PUNTA:
                    ENERGIA = MAX_SOL - (H_ACTUAL - H_PUNTA) * DIV_DELANTE
                elif H_ACTUAL < H_PUNTA:
                    ENERGIA = MAX_SOL - (H_PUNTA - H_ACTUAL) * DIV_DETRAS
                else:
                    ENERGIA = MAX_SOL

                MAX_ALEJ = max(90 - A_OPTIMO, A_OPTIMO)
                DIV_A = MAX_ANG / MAX_ALEJ

                if A_OPTIMO != A_ACTUAL:
                    ENERGIA += MAX_ANG - (abs(A_OPTIMO - A_ACTUAL) * DIV_A)
                else:
                    ENERGIA += MAX_ANG

            return round(ENERGIA, 3)

        except:
            return -1

def setStatus(value):

    try:
        conn = abrirConex()
        cur = conn.cursor()
        sql = "INSERT INTO estado(timestamp, valor) VALUES(?,?)"
        cur.execute(sql, (int(datetime.datetime.now().timestamp()), value))
        conn.commit()
        conn.close()
        return 0
    except:
        return -1

def getStatus():
    try:
        conn = abrirConex()
        cur = conn.cursor()
        cur.execute("SELECT * FROM estado WHERE timestamp = (SELECT MAX(timestamp) FROM estado)")
        r = cur.fetchall()
        return r[0][1]
    except:
        return -1

def getAnguloAct():
    try:
        conn = abrirConex()
        cur = conn.cursor()
        cur.execute("SELECT * FROM registroPlaca WHERE timestamp = (SELECT MAX(timestamp) FROM registroPlaca)")
        r = cur.fetchall()
        return r[0][1]
    except:
        return -1

def reset(mem):
    try:
        conn = abrirConex()
        cur = conn.cursor()

        if mem == 500:
            cur.execute("DROP TABLE horasSol")
        elif mem == 1000:
            cur.execute("DROP TABLE estado")
        elif mem == 1500:
            cur.execute("DROP TABLE angulos")
            cur.execute("DROP TABLE registroPlaca")
        else:
            return -1
            
        conn.commit()
        conn.close()
        return 0
    except:
        return -1