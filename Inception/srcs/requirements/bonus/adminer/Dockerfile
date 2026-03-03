FROM debian:12

RUN apt-get update && apt-get install -y \
	php-fpm \
	php-mysql \
	wget \
	&& rm -rf /var/lib/apt/lists/*

RUN mkdir -p /run/php /var/www/html

RUN wget -O /var/www/html/index.php https://www.adminer.org/latest.php

RUN sed -i 's|listen = /run/php/php8.2-fpm.sock|listen = 9001|' \
    /etc/php/8.2/fpm/pool.d/www.conf

EXPOSE 9001

CMD ["php-fpm8.2", "--nodaemonize"]