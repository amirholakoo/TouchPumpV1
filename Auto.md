To ensure that both `MonitoringV3_HeadLess.py` and `app.py` run automatically on boot and restart if they fail, we can use `systemd`, a system and service manager for Linux. `systemd` allows you to define services, ensure they start on boot, and restart them if they fail.

Here's how to set it up for both scripts:

### 1. Create systemd Service Files:

#### For `MonitoringV3_HeadLess.py`:

Create a new service file:

```bash

sudo nano /etc/systemd/system/garden_monitor.service

```

Add the following content:

```ini

[Unit]

Description=Garden Monitoring Service

After=network.target

[Service]

ExecStart=/usr/bin/python3 /path/to/your/app/MonitoringV3_HeadLess.py

WorkingDirectory=/path/to/your/app

Restart=always

User=pi

[Install]

WantedBy=multi-user.target

```

Replace `/path/to/your/app` with the actual path to your application directory.

#### For `app.py`:

Create another service file:

```bash

sudo nano /etc/systemd/system/garden_app.service

```

Add the following content:

```ini

[Unit]

Description=Garden Flask App Service

After=network.target

[Service]

ExecStart=/usr/bin/gunicorn -b 0.0.0.0:5000 app:app

WorkingDirectory=/path/to/your/app

Restart=always

User=pi

[Install]

WantedBy=multi-user.target

```

Again, replace `/path/to/your/app` with the actual path to your application directory.

### 2. Enable and Start the Services:

Now, you'll enable the services so they start on boot, and then start them:

For `MonitoringV3_HeadLess.py`:

```bash

sudo systemctl enable garden_monitor

sudo systemctl start garden_monitor

```

For `app.py`:

```bash

sudo systemctl enable garden_app

sudo systemctl start garden_app

```

### 3. Check the Status:

You can check the status of your services anytime with:

```bash

sudo systemctl status garden_monitor

```

and

```bash

sudo systemctl status garden_app

```

### 4. Restarting the Services:

If you ever make changes to your scripts and want to restart the services:

```bash

sudo systemctl restart garden_monitor

sudo systemctl restart garden_app

```

With this setup, both `MonitoringV3_HeadLess.py` and `app.py` will start automatically when your Raspberry Pi boots up. Additionally, if either script fails or crashes for some reason, `systemd` will automatically restart it.
