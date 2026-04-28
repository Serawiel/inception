FROM debian:12

RUN apt-get update && apt-get install -y \
	php-fpm \
	php-mysql \
	php-gd \
	php-zip \
	php-redis \
	php-mbstring \
	php-curl \
	php-xml \
	wget \
	unzip \
	&& rm -rf /var/lib/apt/lists/*

RUN mkdir -p /var/www \
	&& wget https://wordpress.org/wordpress-6.4.2.tar.gz -O /tmp/wordpress.tar.gz \
	&& tar -xzf /tmp/wordpress.tar.gz -C /var/www/ \
	&& rm /tmp/wordpress.tar.gz \
	&& mv /var/www/wordpress /var/www/html \
	&& chown -R www-data:www-data /var/www/html

RUN mkdir -p /run/php && chown -R www-data:www-data /run/php

RUN sed -i 's|listen = /run/php/php8.2-fpm.sock|listen = 9000|' \
	/etc/php/8.2/fpm/pool.d/www.conf

WORKDIR /var/www/html

COPY tools/init_wp.sh /init_wp.sh
RUN chmod +x /init_wp.sh

EXPOSE 9000

ENTRYPOINT ["/init_wp.sh"]
