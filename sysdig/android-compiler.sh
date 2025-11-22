#!/bin/bash

sysdig -pc "not proc.name in (systemd, bash, journalctl, sshd) and fd.type=file"
