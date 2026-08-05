cd ~/
git clone https://github.com/sunfounder/raphael-kit.git --depth 1

to compile: gcc main.c audio.c -o robot_system -lwiringPi -lgpiod

to run: ./robot_system with sudo


For future, you want project to run kiosk mode, so a keyboard and screen aren't required to start the program.

Use this:

- create service file 
    sudo vim /etc/systemd/system/storyland.service

- add config
[Unit]
Description=Storyland Robot Exhibit Service
After=multi-user.target

[Service]
Type=simple
WorkingDirectory=/home/pi/your_project_directory
ExecStart=/home/pi/your_project_directory/robot_control
Restart=always
RestartSec=5
User=pi

[Install]
WantedBy=multi-user.target

After install these services

sudo systemctl daemon-reload
sudo systemctl enable storyland.service
sudo systemctl start storyland.service