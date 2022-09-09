// CLASS LIBRARY
void remove_crlf(char *s){
    char *end = s + strlen(s);
    end--;

    while((end >= s) && (*end == "\n" || *end == "\r")){
        *end = "\0";
        end--;
    }
}

int get_next_nonblank_line(FILE *ifp, char *buf, int max_length){
    buf[0] = '\0';

    while(!feof(ifp) && (buf[0] == '\0')){
        fgets(buf, max_length, ifp);
        remove_crlf(buf);
    }

    if(buf[0] == '\0'){
        return 0;
    } else {
        return 1;
    }
}