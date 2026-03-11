# Developer Documentation

This documentation is intended for developers who need to understand, modify, or extend the Inception infrastructure.

## Prerequisites

Before working on this project, ensure you have the following installed and configured:

### Required Software

- **Docker Engine** (20.10+)
- **Docker Compose** (v2.0+)
- **Make** (GNU Make)
- **Git**

### Required Knowledge

- Docker concepts: containers, images, volumes, networks
- Docker Compose orchestration
- Shell scripting (bash/sh)
- SQL basics (for MariaDB)
- NGINX configuration
- PHP-FPM configuration

### System Requirements

- Virtual Machine running Debian 12+ or Alpine (penultimate stable version)
- Minimum 2GB RAM
- 20GB disk space
- Root/sudo access for initial setup

## Environment Setup from Scratch

### 1. Clone the Repository

```bash
git clone <repository_url>
cd inception
```

### 2. Configure `/etc/hosts`

Add your domain to the hosts file:

```bash
echo "127.0.0.1    login.42.fr" | sudo tee -a /etc/hosts
```

Replace `login` with your actual 42 login.

### 3. Create Configuration Files

#### Create `.env` file

Create `srcs/.env` with the following variables:

```bash
DOMAIN_NAME=login.42.fr
WORDPRESS_DB_HOST=mariadb:3306
DATA_PATH=/home/login/data
MYSQL_DATABASE=wordpress_db
```

Replace `login` with your actual login.

#### Create Secrets

Create the `secrets/` directory and secret files:

```bash
mkdir -p secrets
echo "your_db_password" > secrets/db_password.txt
echo "your_root_password" > secrets/db_root_password.txt
echo "your_db_user" > secrets/credentials.txt
```

**Important:** Use strong passwords and never commit these files to Git.

### 4. Verify `.gitignore`

Ensure the following are ignored:

```
secrets/
.env
data/
*.log
.vscode/
```

## Building and Launching the Project

### Using the Makefile

The Makefile provides convenient commands to manage the entire stack:

#### Build and Start Everything

```bash
make
```

This command:
1. Creates data directories at `/home/login/data/`
2. Builds all Docker images from Dockerfiles
3. Starts all containers via `docker-compose up -d --build`

#### Individual Make Targets

```bash
make build    # Build images without starting
make up       # Start containers (no rebuild)
make down     # Stop and remove containers
make stop     # Stop containers only
make clean    # Remove containers and images
make fclean   # Full cleanup (containers, images, volumes, data)
make re       # Rebuild everything from scratch
```

### Using Docker Compose Directly

Navigate to the `srcs/` directory to use docker-compose commands:

```bash
cd srcs

# Start services
docker compose up -d

# Stop services
docker compose down

# View logs
docker compose logs -f

# Restart a specific service
docker compose restart nginx
```

## Managing Containers and Volumes

### Container Management

#### List all containers

```bash
docker ps -a
```

Shows all containers (running and stopped).

#### View container logs

```bash
docker logs srcs-nginx-1
docker logs srcs-wordpress-1
docker logs srcs-mariadb-1
```

Add `-f` to follow logs in real-time.

#### Execute commands inside containers

```bash
# Access a container shell
docker exec -it srcs-nginx-
docker exec -it srcs-wordpress-1

# Run a specific command
docker exec it srcs-mariadb-1 mysql -u wp_user -p
```

#### Restart a specific container

```bash
docker restart srcs-nginx-1
```

#### Inspect container configuration

```bash
docker inspect srcs-wordpress-1
```

### Volume Management

#### List all volumes

```bash
docker volume ls
```

Should show `srcs_wordpress_data` and `srcs_mariadb_data`.

#### Inspect volume details

```bash
docker volume inspect srcs_wordpress_data
docker volume inspect srcs_mariadb_data
```

This shows where data is mounted on the host machine.

#### Remove volumes (⚠️ deletes data)

```bash
docker volume rm srcs_wordpress_data
docker volume rm srcs_mariadb_data
```

### Image Management

#### List all images

```bash
docker images
```

Should show `nginx`, `wordpress`, and `mariadb` images.

#### Remove images

```bash
docker rmi nginx wordpress mariadb
```

#### Rebuild a specific image

```bash
docker compose -f srcs/docker-compose.yml build nginx
```

### Network Management

#### List networks

```bash
docker network ls
```

Should show `srcs_docker_network`.

#### Inspect network

```bash
docker network inspect srcs_docker_network
```

Shows all containers connected to the network and their IP addresses.

## Data Storage and Persistence

### Data Location on Host

All persistent data is stored in `/home/login/data/` on the host machine:

```
/home/login/data/
├── wordpress/          # WordPress files (themes, plugins, uploads)
└── mariadb/           # MariaDB database files
```

Replace `login` with your actual login.

### How Persistence Works

The `docker-compose.yml` defines named volumes with bind mounts:

```yaml
volumes:
  wordpress_data:
    driver: local
    driver_opts:
      type: none
      o: bind
      device: ${DATA_PATH}/wordpress

  mariadb_data:
    driver: local
    driver_opts:
      type: none
      o: bind
      device: ${DATA_PATH}/mariadb
```

**Key points:**
- Named volumes are managed by Docker (`docker volume ls` works)
- Data is stored in a predictable location on the host
- Data persists across container restarts and rebuilds
- Data is NOT deleted by `make down` — only by `make fclean`

### Accessing Persistent Data

#### WordPress Files

```bash
ls -la /home/login/data/wordpress/
```

Contains all WordPress PHP files, themes, plugins, and uploads.

#### MariaDB Database Files

```bash
ls -la /home/login/data/mariadb/
```

Contains MariaDB binary database files (InnoDB, MyISAM).

**Note:** Do not manually edit these files — use SQL commands instead.

## Architecture Overview

### Service Communication

```
Browser (HTTPS port 443)
    │
    ▼
NGINX Container (entry point)
    │
    ├─── Static files ──► /var/www/html (shared volume)
    │
    └─── PHP requests ──► WordPress:9000 (FastCGI)
                              │
                              ▼
                         WordPress Container
                              │
                              └─── Database queries ──► MariaDB:3306
                                                             │
                                                             ▼
                                                        MariaDB Container
```

### Network Configuration

All containers communicate through the `docker_network` bridge network:

- Containers use service names for DNS resolution
- Only NGINX port 443 is exposed to the host
- WordPress and MariaDB are not directly accessible from outside

### Volume Sharing

The `wordpress_data` volume is mounted in both NGINX and WordPress containers:

- **NGINX** reads static files (images, CSS, JS)
- **WordPress** reads and writes PHP files

This allows NGINX to serve static content directly while forwarding PHP requests to WordPress.

## Modifying the Infrastructure

### Adding a New Service

1. Create a new directory under `srcs/requirements/`
2. Write a Dockerfile
3. Add configuration files in `conf/` and scripts in `tools/`
4. Add the service to `docker-compose.yml`
5. Update the Makefile if needed
6. Test with `make fclean && make`

### Changing NGINX Configuration

1. Edit `srcs/requirements/nginx/conf/nginx.conf`
2. Rebuild: `docker compose -f srcs/docker-compose.yml build nginx`
3. Restart: `docker compose -f srcs/docker-compose.yml restart nginx`

### Changing WordPress/PHP Configuration

1. Edit `srcs/requirements/wordpress/tools/init_wp.sh` or Dockerfile
2. Rebuild: `make fclean && make`

### Changing MariaDB Configuration

1. Edit `srcs/requirements/mariadb/tools/init_db.sh` or Dockerfile
2. **Warning:** Rebuilding will reset the database unless you backup first
3. Rebuild: `make fclean && make`

## Additional Resources

- [Docker Documentation](https://docs.docker.com/)
- [Docker Compose Reference](https://docs.docker.com/compose/compose-file/)
- [NGINX Documentation](https://nginx.org/en/docs/)
- [WordPress Developer Resources](https://developer.wordpress.org/)
- [MariaDB Documentation](https://mariadb.org/documentation/)