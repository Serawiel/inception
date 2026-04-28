# User Documentation

This documentation is intended for administrators and end users who need to deploy and manage the Inception infrastructure.

## Services Provided

The Inception stack provides three main services:

1. **WordPress Website** — A content management system for creating and managing web content
2. **MariaDB Database** — Stores all WordPress data (posts, users, settings)
3. **NGINX Web Server** — Serves the website over HTTPS and acts as the entry point to the infrastructure

## Starting and Stopping the Project

### Starting the Infrastructure

Navigate to the project root directory and run:

```bash
make
```

This command will:
- Create necessary data directories
- Build all Docker images
- Start all containers in detached mode

The first launch may take a few minutes as Docker builds the images.

### Stopping the Infrastructure

To stop all services:

```bash
make down
```

This stops and removes all containers but **preserves your data** (WordPress content, database).

To completely stop without removing containers:

```bash
make stop
```

### Restarting

To restart the infrastructure after stopping:

```bash
make up
```

## Accessing the Website

### WordPress Website

Open your web browser and navigate to:

```
https://login.42.fr
```

Replace `login` with your actual 42 login (e.g., `https://cprot.42.fr`).

**Important notes:**
- Only HTTPS works — HTTP (`http://login.42.fr`) is disabled
- You will see a security warning about the self-signed certificate — this is expected, click "Accept the Risk" or "Continue"

### WordPress Administration Panel

To access the admin dashboard:

```
https://login.42.fr/wp-admin
```

Log in using the administrator credentials configured during WordPress setup.

**Administrator account:**
- Username: Does NOT contain "admin" or "Admin" (as required by the project)
- Password: Set during initial WordPress configuration

## Managing Credentials

### Location of Credentials

Credentials are stored in two locations:

1. **Secrets directory** (`secrets/`):
   - `db_password.txt` — MariaDB user password
   - `db_root_password.txt` — MariaDB root password
   - `credentials.txt` — Database username

2. **Environment file** (`srcs/.env`):
   - Domain name
   - Database name
   - Database host
   - Data path

### Changing Credentials

**To change a password:**

1. Edit the corresponding file in `secrets/`:
   ```bash
   echo "new_password" > secrets/db_password.txt
   ```

2. Restart the infrastructure:
   ```bash
   make down
   make
   ```

**Security reminder:** Never commit `secrets/` or `srcs/.env` to Git — they are ignored by `.gitignore`.

## Checking Service Status

### Verify All Services Are Running

```bash
docker ps
```

You should see three containers with status "Up":
- `srcs-nginx-1`
- `srcs-wordpress-1`
- `srcs-mariadb-1`

### Check Individual Service Logs

To view logs for a specific service:

```bash
docker logs srcs-nginx-1
docker logs srcs-wordpress-1
docker logs srcs-mariadb-1
```

Add `-f` flag to follow logs in real-time:

```bash
docker logs -f srcs-wordpress-1
```

### Verify Network Connectivity

Test if NGINX is responding:

```bash
curl -k https://login.42.fr
```

The `-k` flag ignores the self-signed certificate warning.

### Check Database Connection

Connect to MariaDB to verify it's running:

```bash
docker exec -it srcs-mariadb-1 mysql -u wp_user -p
```

Enter the password from `secrets/db_password.txt` when prompted.

Inside MySQL prompt, verify the database exists:

```sql
SHOW DATABASES;
USE wordpress_db;
SHOW TABLES;
EXIT;
```

### Data Persistence After Reboot

After rebooting your virtual machine:

1. Navigate to the project directory
2. Run `make up` to restart containers
3. Your WordPress content and database should remain intact

Data is stored in `/home/login/data/` on the host machine and persists across container restarts.

## Complete Cleanup

To remove everything (containers, images, volumes, and data):

```bash
make fclean
```

**Warning:** This will delete all WordPress content and database data. Only use this if you want to start fresh.