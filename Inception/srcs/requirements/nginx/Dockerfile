FROM debian:11

RUN apt-get update && apt-get install -y \
    nginx \
    openssl \
    && rm -rf /var/lib/apt/lists/*

RUN openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
    -keyout /etc/ssl/private/mysite.key \
    -out /etc/ssl/certs/mysite.crt \
    -subj "/CN=cprot.42.fr"

RUN mkdir -p /etc/nginx/conf.d /var/log/nginx /var/www/html

COPY conf/nginx.conf /etc/nginx/conf.d/default.conf

EXPOSE 443

ENTRYPOINT ["nginx", "-g", "daemon off;"]