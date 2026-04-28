# Inception

_This project has been created as part of the 42 curriculum by cprot._

## Description

Inception is a system administration project that aims to set up a complete web infrastructure locally using Docker. The goal is to deploy a WordPress website with its database and web server, all running in separate containerized services.

The infrastructure consists of three main services:
- **NGINX**: A web server that acts as the single entry point, handling HTTPS connections and serving as a reverse proxy
- **WordPress + PHP-FPM**: The content management system with PHP FastCGI Process Manager for dynamic content processing
- **MariaDB**: The database server that stores all WordPress data

All services run in isolated Docker containers and communicate through a dedicated Docker network, ensuring security and modularity. Data persistence is achieved through Docker volumes mounted on the host machine.

## Instructions

### Prerequisites

1. **Virtual Machine Setup**
   - Create a VM using your preferred virtualization software (e.g., VMM, VirtualBox)
   - Recommended specifications:
     - OS: as you wish !
     - RAM: 2048 MB
     - CPU: 2 cores
     - Storage: 20 GB
   - Install Docker and sudo inside the VM

2. **Domain Configuration**
   - Add the following line to `/etc/hosts` on your VM (requires root privileges):
     ```bash
     echo "127.0.0.1    login.42.fr" | sudo tee -a /etc/hosts
     ```
     Replace `login` with your actual 42 login. This maps your domain to localhost.

### Project Structure

Clone the repository and ensure the following directory structure (as specified in the subject), be careful about the permissions !

```
.
├── Makefile
├── .gitignore
├── README.md
├── USER_DOC.md
├── DEV_DOC.md
├── secrets/              # Git-ignored
│   ├── db_password.txt
│   ├── db_root_password.txt
│   └── credentials.txt
└── srcs/
    ├── docker-compose.yml
    ├── .env              # Git-ignored - must be created manually
    └── requirements/
        ├── nginx/
        │   ├── Dockerfile
        │   ├── conf/
        │   │   └── nginx.conf
        │   └── tools/
        ├── wordpress/
        │   ├── Dockerfile
        │   ├── conf/
        │   └── tools/
        │       └── init_wp.sh
        └── mariadb/
            ├── Dockerfile
            ├── conf/
            └── tools/
                └── init_db.sh
```

### Security Configuration

Before starting, ensure you have a `.gitignore` file at the root containing. The subject say : 
**Any credentials, API keys, or passwords found in your Git repository will result in project failure.**

### Installation & Execution

1. **Clone the repository:**

2. **Create the `.env` file** in `srcs/` with your environment variables. Create the makefile with all the docker commands (build, run but also to stop and clean).

3. **Build and launch the infrastructure:**
   ```bash
   make
   ```
   This command will:
   - Create necessary directories in `/home/login/data/wordpress` and `/home/login/data/mariadb`
   - Build all Docker images from scratch (no pre-built images allowed)
   - Start all containers via docker-compose in detached mode

4. **Access the WordPress website:**
   - Open your browser and navigate to: `https://login.42.fr`
   - **Important:** 
     - Only HTTPS (port 443) is configured - HTTP will not work
     - Accept the self-signed certificate warning (this is expected behavior)
     - You should see the configured WordPress website (not the installation page)

### Verification

To verify all services are running:
```bash
docker ps
```

You should see 3 containers: `nginx`, `wordpress`, and `mariadb`.

## Resources

### Docker Documentation
- [Docker Overview](https://docs.docker.com/get-started/docker-overview/)
- [Docker Compose Documentation](https://docs.docker.com/compose/)
- [Dockerfile Best Practices](https://docs.docker.com/develop/develop-images/dockerfile_best-practices/)

### Tutorials & Articles
- [Understanding Docker (French)](https://www.nicelydev.com/docker/docker-comprendre)
- [Docker Container Engines (French)](https://blog.stephane-robert.info/docs/conteneurs/moteurs-conteneurs/docker/)

### AI Usage
During this project, AI (Claude) was used for:
- **Debugging**: Understanding Docker networking issues and container communication problems
- **Concept clarification**: Learning about TLS certificates, FastCGI protocol, and Docker volume management
- **Best practices**: Guidance on Dockerfile optimization and security configurations
- **Documentation**: Assistance in structuring README and understanding technical concepts
All AI-generated content was thoroughly reviewed, tested, and validated before integration into the project.

## Project Description

### Docker Overview

This project uses Docker to create a lightweight, reproducible infrastructure. Each service runs in its own container, providing isolation and portability.

**Key Docker concepts used:**
- **Containers**: Isolated environments running specific services
- **Images**: Templates built from Dockerfiles
- **Volumes**: Persistent storage for database and WordPress files
- **Networks**: Internal communication between containers
- **docker-compose**: Orchestration tool to manage multi-container applications

### Design Choices

1. **Three-tier architecture**: Separation of web server (NGINX), application layer (WordPress/PHP-FPM), and database (MariaDB)
2. **Custom Dockerfiles**: All images built from scratch to ensure full control and understanding
3. **Named volumes with bind mounts**: Data persists in `/home/login/data` on the host machine
4. **TLS-only access**: HTTPS enforced with self-signed certificates (TLSv1.2/TLSv1.3)
5. **Environment variables**: Credentials stored in `.env` file, not in code

### Technical Comparisons

#### Virtual Machines vs Docker

| Aspect | Virtual Machines | Docker Containers |
|--------|------------------|-------------------|
| **Architecture** | Full OS with hypervisor | Shared kernel, isolated processes |
| **Size** | Gigabytes (entire OS) | Megabytes (application + dependencies) |
| **Boot time** | Minutes | Seconds |
| **Resource usage** | Heavy (dedicated RAM/CPU) | Lightweight (shared resources) |
| **Isolation** | Complete hardware virtualization | Process-level isolation |
| **Portability** | Limited (depends on hypervisor) | High (runs anywhere Docker runs) |
| **Use case** | Different OS requirements, strong isolation | Microservices, CI/CD, development environments |

**For this project**: Docker is preferred for its lightweight nature, fast deployment, and ease of replication across different environments.

#### Secrets vs Environment Variables

| Aspect | Environment Variables | Docker Secrets |
|--------|----------------------|----------------|
| **Storage** | `.env` file or docker-compose | Encrypted in Docker Swarm |
| **Security** | Plaintext (must be git-ignored) | Encrypted at rest and in transit |
| **Access** | Available as env vars in container | Mounted as files in `/run/secrets/` |
| **Scope** | docker-compose only | Docker Swarm/Kubernetes |
| **Rotation** | Manual file edit | Automated rotation possible |

**For this project**: Both environment variables (.env file) and Docker Secrets are used. The subject strongly recommends using Docker Secrets, which can be encrypted when using Docker Swarm (not the case here). However, it allows us to separate sensitive data (passwords, credentials) from regular environment variables (domain names, configuration).

#### Docker Network vs Host Network

| Aspect | Docker Network (bridge) | Host Network |
|--------|------------------------|--------------|
| **Isolation** | Containers have their own network namespace | Container uses host's network directly |
| **Port mapping** | Required (e.g., `443:443`) | Not needed (direct host port access) |
| **Container communication** | Via container names (DNS resolution) | Via `localhost` |
| **Security** | Isolated, controlled exposure | Full host network access |
| **Performance** | Slight overhead (NAT) | Native performance |

**For this project**: A dedicated Docker network (`docker_network`) is used to:
- Isolate containers from the host network
- Enable DNS-based service discovery (e.g., `wordpress:9000`, `mariadb:3306`)
- Control which ports are exposed to the host (only NGINX on 443)

#### Docker Volumes vs Bind Mounts

| Aspect | Named Volumes | Bind Mounts |
|--------|---------------|-------------|
| **Management** | Managed by Docker | User manages host path |
| **Portability** | Location-agnostic | Hardcoded host paths |
| **Permissions** | Docker handles permissions | Manual permission management |
| **Backup** | Docker volume commands | Standard file backup |
| **Performance** | Optimized by Docker | Direct host filesystem |

**For this project**: Named volumes with `driver_opts` (bind type) are used — a hybrid approach that:
- Uses Docker volume management (`docker volume ls`, `docker volume inspect`)
- Stores data in a predictable location (`/home/login/data`)
- Allows easy backup and inspection of files
- Satisfies the subject requirement for named volumes in a specific location

This approach combines the benefits of Docker volume management with the transparency of knowing exactly where data is stored on the host.