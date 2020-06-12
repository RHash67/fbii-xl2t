## FBII XL-2T Web Interface (for Raspberry Pi)

I am using a Raspberry Pi Zero W for the web interface, as it is small, power efficient, and has built-in wifi.  This
web interface program/files assumes that an arduino is used as the hardware interface to the alarm system, and that a
serial connection is used from the arduino to the Raspberry Pi.

This python file (app.py) is adapted from the example code from the online Flask documentation.

You should make a subfolder in your root directory and put the app.py file in there.
Put the "index.html" and "login.html" files in a "templates" subfolder.  Put the "style.css" file in a "static"
subfolder.  So your directory tree should look like this (in this example the root subfolder is called "web-server"):

web-server
- app.py
- templates
  - index.html
  - login.html
- static
  - style.css
     
I have retained the directory structure in this repository for clarity.

I made environment variable pointers in the app.py file for the confidential info instead of embedding the info in the
code.  I did this so if you copy the app.py code or share it with someone, your confidential information won't
be included with the code.  You'll need to set the environment variables in the .bashrc file which the app.py references.
To do this, edit the .bashrc file in your root directory, and add the following lines:

export EMAIL_USER=*youremailusername*  (this is the email account UN that the alarm notification will originate from)

export EMAIL_PASSWORD=*youremailpassword*  (and email account password)

export EMAIL_ADDRESS=*emailsendaddress* (this is the email address that alarm notification will be sent to)

export XL2T_USER=*webusername*

export XL2T_PW=*webpassword*

Replace the italicized text with your information.  For example, if your email account user name is "johndoe@mysite.com",
the first line would be:
export EMAIL_USER=johndoe@mysite.com

You will need to enter your email server site in the mail settings area of the app.py file.  You may also have to change
the security settings (port, TLS, etc) to suit your particular email account server.

After changing the .bashrc file, you will need to reboot the Raspberry Pi to activate the variables.

You will also need to install the following packages:

- pip install flask
- pip install flask-socketio
- pip install Flask-Mail
- pip install flask-login
- apt-get install libevent-dev
- apt-get install python-all-dev
- pip install greenlet
- apt-get install libffi6 libffi-dev
- pip install gevent-websocket  [this one may take awhile to install]
- pip install pyserial

You will need to enable serial communication in the "sudo raspi-config" option.

After you've done the above, go to your subdirectory and type in "python app.py" at the command prompt.  If all goes well,
you should see the app run messages (restarting with stat, debugger active, and debugger PIN), and no error messages.  This
means the system is waiting for a webuser to login to the interface.

On another device, you should be able to see the web interface via the IP address of your Raspberry Pi, on port 5000 (for
example, if your Raspberry Pi's IP is 192.168.1.20, type in "192.168.1.20:5000" in your web browser address bar.  You may
need to set port forwarding in your router.  You should be greeted with the interface login page.  Enter the username and
password (which you set in the "XL2T" environment variables), and that should bring you to the alarm interface screen.  Note
that the user name and password are case-sensitive.

