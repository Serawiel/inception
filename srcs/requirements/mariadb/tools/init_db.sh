#!/bin/sh

# Read secrets from Docker secrets files
MYSQL_ROOT_PASSWORD=$(cat /run/secrets/db_root_password)
MYSQL_PASSWORD=$(cat /run/secrets/db_password)
MYSQL_USER=$(cat /run/secrets/db_user)

# Check if MariaDB is already initialized
if [ ! -d "/var/lib/mysql/mysql" ]; then

	echo "Initializing MariaDB for the first time..."
	mysql_install_db --user=mysql --datadir=/var/lib/mysql
	mysqld --user=mysql --bootstrap << EOF
USE mysql;
FLUSH PRIVILEGES;

DELETE FROM mysql.user WHERE User='';
DELETE FROM mysql.user WHERE User='root' AND Host NOT IN ('localhost', '127.0.0.1', '::1');

ALTER USER 'root'@'localhost' IDENTIFIED BY '${MYSQL_ROOT_PASSWORD}';

CREATE DATABASE IF NOT EXISTS \`${MYSQL_DATABASE}\`;

CREATE USER IF NOT EXISTS '${MYSQL_USER}'@'%' IDENTIFIED BY '${MYSQL_PASSWORD}';

GRANT ALL PRIVILEGES ON \`${MYSQL_DATABASE}\`.* TO '${MYSQL_USER}'@'%';

FLUSH PRIVILEGES;
EOF

	echo "MariaDB successfully initialized!"

else
	echo "MariaDB already initialized, starting..."
fi

echo "Starting MariaDB server..."
exec mysqld --user=mysql --console --bind-address=0.0.0.0