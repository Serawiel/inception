FROM debian:11

RUN apt-get update && apt-get install -y \
	php-fpm \
	php-mysql \
	wget \
	&& rm -rf /var/lib/apt/lists/*

RUN wget -O /var/www/html/index.php https://www.adminer.org/latest.php

RUN sed -i 's|listen = /run/php/php7.4-fpm.sock|listen = 9001|' \
    /etc/php/7.4/fpm/pool.d/www.conf

RUN mkdir -p /run/php /var/www/html

EXPOSE 9001

CMD ["php-fpm7.4", "--nodaemonize"]