#!/usr/bin/env python3
from threading import Lock
from flask import Flask, render_template, request, flash, redirect, \
    session, abort
from flask_socketio import SocketIO, emit, disconnect
from flask_mail import Mail, Message
import os
import serial
import time

# Set this variable to "threading", "eventlet" or "gevent" to test the
# different async modes, or leave it set to None for the application to choose
# the best option based on installed packages.
async_mode = None

port = serial.Serial(port='/dev/serial0', baudrate=38400, timeout=0.5)

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, async_mode=async_mode)
thread = None
thread_lock = Lock()

mail_settings = {
    "MAIL_SERVER": os.environ['EMAIL_SERVER'],
    "MAIL_PORT": 587,
    "MAIL_USE_TLS": True,
    "MAIL_USE_SSL": False,
    "MAIL_USERNAME": os.environ['EMAIL_USER'],
    "MAIL_PASSWORD": os.environ['EMAIL_PASSWORD']
    }

app.config.update(mail_settings)
mail = Mail(app)

def background_thread():  # This function runs on connection to client
    # This is how to send server generated events to clients.
    prevbuzzer = "0"
    emailsent = 0
    buzzinittime = time.time()
    emailto = os.environ['EMAIL_ADDRESS']

    while True:  # keep looping
        socketio.sleep(1)
        port.reset_input_buffer()
        port.reset_output_buffer()
        port.write('@'.encode())
        rcv=' '
        rcv=port.read(size=5)
        valdict=parse_data(rcv)
        if (valdict["buzzer"] == "1") and (prevbuzzer == "0"):
            buzzinittime = time.time()
            prevbuzzer = "1"
        if (valdict["buzzer"] == "0") and (prevbuzzer == "1"):
            prevbuzzer = "0"
            emailsent = 0
        if (valdict["buzzer"] == "1") and (prevbuzzer == "1") and (time.time() - buzzinittime > 5):
            valdict["alarm"] = "#FF0000"  # check if buzzer has been on > 5 sec.  If so, turn on alarm indicator
            if emailsent < 2:
                with app.app_context():
                    msg = Message(subject="House",
                                  sender=app.config.get("MAIL_USERNAME"),
                                  recipients=[emailto],
                                  body="Alarm has been set off!")
                    mail.send(msg)
                    emailsent += 1
        socketio.emit('my_response', valdict, namespace='/test')

def parse_data(data):  # this function sets the individual indicator colors based on the alarm status bytes
    if (data == '\x00\x00\x00\x00\x00') or (len(data) < 4):  # no or invalid data received
        data = "32768"
    dataint=int(data)
    tmpdict = {"buzzer": "0", "alarm": "#000000"}
    tmpval = 0x8000 & dataint
    if tmpval != 0:
        tmpdict["buzzer"] = "0"
    else:
        tmpdict["buzzer"] = "1"
    tmpval = 0x0800 & dataint
    if tmpval != 0:
        tmpdict["arm"] = "#FF0000"
    else:
        tmpdict["arm"] = "#000000"
    tmpval = 0x1000 & dataint
    if tmpval != 0:
        tmpdict["stay"] = "#FF0000"
    else:
        tmpdict["stay"] = "#000000"
    tmpval = 0x2000 & dataint
    if tmpval != 0:
        tmpdict["instant"] = "#FF0000"
    else:
        tmpdict["instant"] = "#000000"
    tmpval = 0x0200 & dataint
    if tmpval != 0:
        tmpdict["ac"] = "#FFAA00"
    else:
        tmpdict["ac"] = "#000000"
    tmpval = 0x4000 & dataint
    if tmpval != 0:
        tmpdict["ready"] = "#00FF00"
    else:
        tmpdict["ready"] = "#000000"
    tmpval = 0x0008 & dataint
    if tmpval != 0:
        tmpdict["z1"] = "#FF0000"
    else:
        tmpdict["z1"] = "#000000"
    tmpval = 0x0010 & dataint
    if tmpval != 0:
        tmpdict["z2"] = "#FF0000"
    else:
        tmpdict["z2"] = "#000000"
    tmpval = 0x0020 & dataint
    if tmpval != 0:
        tmpdict["z3"] = "#FF0000"
    else:
        tmpdict["z3"] = "#000000"
    tmpval = 0x0040 & dataint
    if tmpval != 0:
        tmpdict["z4"] = "#FF0000"
    else:
        tmpdict["z4"] = "#000000"
    tmpval = 0x0080 & dataint
    if tmpval != 0:
        tmpdict["z5"] = "#FF0000"
    else:
        tmpdict["z5"] = "#000000"
    tmpval = 0x0100 & dataint
    if tmpval != 0:
        tmpdict["z6"] = "#FF0000"
    else:
        tmpdict["z6"] = "#000000"
    return tmpdict

@app.route('/')
def home():
    if not session.get('logged_in'):
        return render_template('login.html')
    else:
        return render_template('index.html', async_mode=socketio.async_mode)

@app.route('/login', methods=['POST'])
def do_admin_login():
    if request.form['password'] == os.environ['XL2T_PW'] and request.form['username'] == os.environ['XL2T_USER']:
        session['logged_in'] = True
    else:
        flash('wrong password!')
    return home()

@app.route('/logout')
def logout():
    session['logged_in'] = False
    return home()

@socketio.on('send_key', namespace='/test')  # send keypress from client webpage
def send_key(key):
    port.reset_output_buffer()
    port.write(key['data'].encode())

@socketio.on('connect', namespace='/test')
def test_connect():
    global thread
    with thread_lock:
        if thread is None:
            thread = socketio.start_background_task(background_thread)

@socketio.on('disconnect', namespace='/test')
def test_disconnect():
    session['logged_in'] = False
    print('Client disconnected', request.sid)

if __name__ == '__main__':
    socketio.run(app, debug=True, host='0.0.0.0')
