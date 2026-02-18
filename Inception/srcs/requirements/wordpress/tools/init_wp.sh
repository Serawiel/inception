#!/bin/sh
set -e

# Read secrets from Docker secrets files
MYSQL_PASSWORD=$(cat /run/secrets/db_password)
MYSQL_USER=$(cat /run/secrets/db_user)

# Configure WordPress if not already done
if [ ! -f /var/www/html/wp-config.php ]; then
    echo "Configuring WordPress..."
    cp /var/www/html/wp-config-sample.php /var/www/html/wp-config.php

    sed -i "s/database_name_here/${MYSQL_DATABASE}/" /var/www/html/wp-config.php
    sed -i "s/username_here/${MYSQL_USER}/" /var/www/html/wp-config.php
    sed -i "s/password_here/${MYSQL_PASSWORD}/" /var/www/html/wp-config.php
    sed -i "s/localhost/${WORDPRESS_DB_HOST}/" /var/www/html/wp-config.php

    echo "WordPress configured!"
fi

# Install WP-CLI if not already installed
if [ ! -f /usr/local/bin/wp ]; then
    echo "Installing WP-CLI..."
    wget -q https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar -O /tmp/wp-cli.phar
    chmod +x /tmp/wp-cli.phar
    mv /tmp/wp-cli.phar /usr/local/bin/wp
    echo "WP-CLI installed!"
fi

# Wait for MariaDB to be ready
echo "Waiting for MariaDB..."
sleep 10

# Configure Redis
if ! grep -q "WP_REDIS_HOST" /var/www/html/wp-config.php; then
    echo "Configuring Redis..."
    sed -i "/\/\* That's all, stop editing/i \
/* Redis Cache */\n\
define('WP_REDIS_HOST', 'redis');\n\
define('WP_REDIS_PORT', 6379);\n" /var/www/html/wp-config.php
    echo "Redis configured!"
fi

# Install and activate Redis Object Cache plugin
if ! wp plugin is-installed redis-cache --allow-root --path=/var/www/html; then
    echo "Installing Redis Cache plugin..."
    wp plugin install redis-cache --activate --allow-root --path=/var/www/html
    echo "Redis Cache plugin installed!"
fi

# Enable Redis cache
if ! wp redis status --allow-root --path=/var/www/html 2>/dev/null | grep -q "Connected"; then
    echo "Enabling Redis cache..."
    wp redis enable --allow-root --path=/var/www/html 2>/dev/null || true
    echo "Redis cache enabled!"
fi

echo "Starting PHP-FPM..."
exec php-fpm7.4 --nodaemonize