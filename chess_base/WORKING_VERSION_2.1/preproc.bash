cat PGNfile | grep -v "\[" | tail -n +1 |tr "." "\n"|tail -n +2 | cut -d " " -f1,2 | tr " " "\n" | head -n -1
