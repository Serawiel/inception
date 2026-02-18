FROM debian:11

RUN apt-get update && apt-get install \
	vsftpd \
	&& rm -rf /var/lib/apt/lists/*

COPY tools/init_vsftpd.sh /init_vsftpd.sh
RUN chmod +x /init_vsftpd.sh

EXPOSE 21

ENTRYPOINT ["/init_vsftpd.sh"]