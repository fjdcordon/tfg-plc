import time
from apscheduler.schedulers.background import BackgroundScheduler
from funciones import moverPlaca, getAnguloOpt

sched = BackgroundScheduler()

def update():
    moverPlaca(getAnguloOpt())

def main():
    sched.add_job(update, trigger='cron', minute='0', hour='0', day='1')
    sched.start()

    try:
        while True:
            time.sleep(2)
    except (KeyboardInterrupt, SystemExit):
        sched.shutdown()

if __name__ == '__main__':
    main()