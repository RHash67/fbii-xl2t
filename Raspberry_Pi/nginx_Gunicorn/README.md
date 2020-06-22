## Web Interface using an nginx/Gunicorn Server##

It is possible to host the Flask-based interface using an nginx/Gunicorn server (instead of using the Flask built-in server).  I'm not sure if there are any performance-based benefits for this application by doing this.  For my system I'm choosing to do this since one of the benefits I notice is that the server and interface starts up automatically on RPi boot up.  With the Flask server, I need to manually run the python code from the command line.

I used the procedure outlined in https://www.e-tinkers.com/2018/08/how-to-properly-host-flask-application-with-nginx-and-guincorn/ as a basis for setting up the interface.  I made some modifications to the procedure as outlined below.  The basic steps I did were:

1.  Install the Flask and related Python 3 packages per the README file in the Raspberry_Pi folder.  You should test your setup with the Flask server just to make sure the program and interface works.
2.  Install the nginx server package (sudo apt install nginx).
3.  Adjust the nginx.conf file per the e-tinkers site procedure (optional - not sure if this helps for this application but I do it anyway).
4.  Add/configure the nginx server directive file:
    a.  Copy the XL2Tint file to the /etc/nginx/sites-available/ folder
    b.  Point the symbolic link to the XL2Tint file:
        i.  Go to /etc/nginx/sites-enabled/ folder
        ii.  sudo rm default
        iii.  sudo ln -s /etc/nginx/sites-available/XL2Tint
5.  UNDER CONSTRUCTION
