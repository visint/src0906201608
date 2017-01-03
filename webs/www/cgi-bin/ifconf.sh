#!/bin/sh
echo "Content-type: text/html"
echo ""
echo "<html><head><meta http-equiv='Content-Type' content='text/html; charset=utf-8' /><title>ifconfig"
echo "</title></head><body>"
echo "<h3>ifconfig</h3>"
echo "<pre> $(ifconfig) </pre>"
echo "</body></html>"

