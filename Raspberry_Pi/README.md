## FBII XL-2T Web Interface (for Raspberry Pi)

I am using a Raspberry Pi Zero W for the web interface, as it is small, power efficient, and has built-in wifi.  This web interface program/files assumes that an arduino is used as the hardware interface to the alarm system, and that a serial connection is used from the arduino to the Raspberry Pi.  Refer to the arduino and circuit folders for more information.

I chose to use a Flask framework as a basis for the program, since it has support for asynchronous events originating from the server (alarm system) side (for example, when zones are triggered).  The main python file (app.py) is adapted from the example code from the online Flask documentation.

Make sure you change the default password on your RPi to deter unauthorized access to the code and other sensitive information.  You may want to change the default user name as well.

The procedure listed here should get the system up and running using the built-in web server included with Flask.  The documentation says that this is only a development-type server, and not intended for "production use", but I think they are talking about multiple-client type applications such as chat rooms where many users are logging in.  For this system I am the only one who will be using the interface, so I think there should be no problem in using the Flask server for the interface.

Make a subfolder in your root directory and put the app.py file in there.  Put the "index.html" and "login.html" files in a "templates" subfolder.  Put the "style.css" file in a "static" subfolder.  So your directory tree should look like this (in this example the root subfolder is called "web-server"):

web-server
- app.py
- templates
  - index.html
  - login.html
- static
  - style.css
     
I have retained the directory structure in this repository for clarity.

I made environment variable pointers in the app.py file for the confidential info instead of embedding the info in the code.  I did this so if you copy the app.py code or share it with someone, your confidential information won't be included with the code.  You'll need to set the environment variables in the .bashrc file which the app.py references.  To do this, edit the .bashrc file in your root directory, and add the following lines:

export EMAIL_SERVER=*youremailserver*  (the is the email server that the alarm notification will originate from)

export EMAIL_USER=*youremailusername*  (this is the email account user name that the alarm notification will originate from.  This is often just the account email address)

export EMAIL_PASSWORD=*youremailpassword*

export EMAIL_ADDRESS=*emailsendaddress* (this is the email address that the alarm notification will be sent to)

export XL2T_USER=*webusername* (the user name for the web interface login.  Case sensitive)

export XL2T_PW=*webpassword* (the password for the web interface login.  Case sensitive)

Replace the italicized text with your information.  For example, if your email account user name is "johndoe@mysite.com",
the first line would be:
export EMAIL_USER=johndoe@mysite.com

You may also have to change the security settings (port, TLS, etc) to suit your particular email account server in the mail settings area of the app.py file.

After changing the .bashrc file, you will need to reboot the Raspberry Pi to activate the variables.

You will also need to install the following packages:

- sudo apt update (this will update your system package list)
- sudo apt install python3-pip
- sudo pip3 install flask
- sudo pip3 install flask-socketio
- sudo pip3 install Flask-Mail
- sudo pip3 install flask-login
- sudo apt install libevent-dev
- sudo apt install python-all-dev
- sudo pip3 install greenlet
- sudo apt install libffi6 libffi-dev
- sudo pip3 install gevent-websocket  [this one may take awhile to install - be patient and just wait for it to complete]
- sudo pip3 install pyserial

You can use the corresponding Python 2 packages instead of the Python 3 packages listed above - however, using the Python 3 packages will set up your RPi to use an nginx/Gunicorn based server if you decide to move to that setup (I have found during my testing that Gunicorn does not work with the Python 2 packages).

You will need to enable serial communication in the "sudo raspi-config" option.

After you've done the above, go to your subdirectory and type in "python3 app.py" at the command prompt.  If all goes well,
you should see the app run messages (restarting with stat, debugger active, and debugger PIN), and no error messages.  This
means the system is waiting for a webuser to login to the interface.

On another device, you should be able to see the web interface via the IP address of your Raspberry Pi, on port 5000 (for
example, if your Raspberry Pi's IP is 192.168.1.20, type in "192.168.1.20:5000" in your web browser address bar.  You may
need to set port forwarding in your router.  You should be greeted with the interface login page.  Enter the username and
password (which you set in the "XL2T" environment variables), and that should bring you to the alarm interface screen.  Note
that the user name and password are case-sensitive.

For the web interface testing I used the "Interface_simulation.ino" sketch in the arduino folder in order to avoid having to connect the actual alarm system.  The web interface files/procedure described above is for a Flask setup.  My understanding is that Flask is intended as a development tool, rather than use as a "real" production-type server.  I am trying to deploy the interface to a Gunicorn/nginx platform - if I am successful I will post the config files.

