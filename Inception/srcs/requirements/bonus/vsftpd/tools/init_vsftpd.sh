#!/bin/sh
set -e

# Read secrets
FTP_USER=$(cat /run/secrets/ftp_user)
FTP_PASSWORD=$(cat /run/secrets/ftp_password)

# Create FTP user if it doesn't exist
if ! id "$FTP_USER" >/dev/null 2>&1; then
    echo "Creating FTP user..."
    useradd -m -d /var/www/html "$FTP_USER"
    echo "$FTP_USER:$FTP_PASSWORD" | chpasswd
    chown -R "$FTP_USER:$FTP_USER" /var/www/html
    echo "FTP user created!"
fi

mkdir -p /var/run/vsftpd/empty

# Configure vsftpd
cat > /etc/vsftpd.conf << EOF
listen=YES
anonymous_enable=NO
local_enable=YES
write_enable=YES
chroot_local_user=YES
allow_writeable_chroot=YES
seccomp_sandbox=NO
pasv_enable=YES
pasv_min_port=21100
pasv_max_port=21110
EOF

echo "Starting vsftpd..."
exec /usr/sbin/vsftpd /etc/vsftpd.conf