#!/bin/bash

sqlite3 ./gen.db <<EOF
CREATE TABLE IF NOT EXISTS match_files (a integer, p text);
.exit
EOF
