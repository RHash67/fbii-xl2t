## Web Interface using an nginx/Gunicorn Server

It is possible to host the Flask-based interface using an nginx/Gunicorn server (instead of using the Flask built-in server).  I'm not sure if there are any performance-based benefits for this application by doing this.  For my system I'm choosing to do this since one of the benefits I notice is that the server and interface starts up automatically on RPi boot up.  With the Flask server, I need to manually run the python code from the command line.

I used the procedure outlined in [this site](https://www.e-tinkers.com/2018/08/how-to-properly-host-flask-application-with-nginx-and-guincorn/) as a basis for setting up the interface.  I made some modifications to the procedure as outlined below.  The basic steps I did were:

1. Install the Flask and related Python 3 packages per the README file in the Raspberry_Pi folder.  You should test your setup with the Flask server just to make sure the program and interface works.
2. Install the nginx server package (sudo apt install nginx).
3. Adjust the nginx.conf file per the e-tinkers site procedure (optional - not sure if this helps for this application but I do it anyway).
4. Add/configure the nginx server directive file:
   - Copy the XL2Tint file to the /etc/nginx/sites-available/ folder
   - Point the symbolic link to the XL2Tint file:
     - Go to /etc/nginx/sites-enabled/ folder
     - sudo rm default
     - sudo ln -s /etc/nginx/sites-available/XL2Tint
   - Test the nginx configuration (sudo nginx -t)
   - If tests OK, then reload the nginx config (sudo service nginx reload)
5. Copy the XL2T.py file, and the static and templates folders from the Raspberry_Pi folder, into /var/www/html/.  This is where the nginx server will host the site from.  The XL2T.py file is the same as the app.py file, except that the async mode has been forced to use gevent.  I renamed the file, to avoid confusion with procedure calls that use the name "app".
6. Install the Gunicorn package (sudo pip3 install gunicorn).
7. Copy the gunicorn.service file into /etc/systemd/system/.  This is similar to the gunicorn.service shown in the e-tinkers site, except that 1) the pointers have been changed to refer to the XL2T.py file, 2) group name has been changed to sudo, 3) the ExecStart command has been changed to use gevent with the Flask websocket, and 4) a pointer to an environment variable file has been added.
8. Copy the envvar file to the /var/ folder, and fill in the variable information in the file (similar to modifyinng the .bashrc script with export commands mentioned in the Flask setup area).  You do not have to use export commands in the envvar file).  NOTE:  You can save the envvar file somewhere else (and rename it) if you prefer - if you do this, make sure you adjust the location (and name) pointer in the gunicorn.service file.
9. Enable the gunicorn service (sudo systemctl enable gunicorn) so it can start at boot, and start the service (sudo systemctl start gunicorn).
10. Check if the gunicorn service is running properly (sudo systemctl status gunicorn).  If all is well, you will see a list of processes running and no error messages.
11. That's it!  You are done.  The web interface is accessed via the IP address of the Raspberry Pi, on port 80.
