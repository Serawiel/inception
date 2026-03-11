FROM debian:12

RUN apt-get update && \
	apt-get install -y mariadb-server mariadb-client && \
	rm -rf /var/lib/apt/lists/*

RUN rm -rf /var/lib/mysql/*

RUN mkdir -p /var/lib/mysql /run/mysqld
RUN chown -R mysql:mysql /var/lib/mysql /run/mysqld

COPY tools/init_db.sh /tmp/init_db.sh
RUN chmod +x /tmp/init_db.sh

EXPOSE 3306

ENTRYPOINT ["/tmp/init_db.sh"]