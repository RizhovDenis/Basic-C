static void write_buffer(bitFILE *bitF){
    int result;

    result = fwrite(bitF->buffer, 1, bitF->bytepos, bitF->file);
    if(result != bitF->bytepos){
        return;
    }

    /* clear the buffer */
    bitF->bytepos = 0;
    bitF->bitpos = 0;
    memset(bitF->buffer, 0, BIT_IO_BUFFER);
}

static int bitIO_write(bitFILE *bitF, void *info, int nbit){
    unsigned char mask;
    int i;
    int byte_pos = 0, bit_pos = 0;
    for(i = 0; i < nbit; i++){
        mask = 1 << bit_pos; // get bit to write
    
        // if it is a 1 set it, otherwise do nothing
        if((*(unsigned char *)(info + byte_pos) & mask) != 0){
            bitF->buffer[bitF->bytepos] |= (1 << bitF->bitpos);
        }
        /* update info to write variables */
        byte_pos = (bit_pos < 7) ? byte_pos : (byte_pos + 1);
        bit_pos = (bit_pos < 7) ? (bit_pos + 1) : 0;
        /* update bitF structure */
        bitF->bytepos = (bitF->bitpos < 7) ? bitF->bytepos : (bitF->bytepos + 1);
        bitF->bitpos = (bitF->bitpos < 7) ? (bitF->bitpos + 1) : 0;
        /* check if a write_buffer must be done */
        if (bitF->bytepos == BIT_IO_BUFFER){
            write_buffer(bitF);
        }
    }
    return i;
}

static int bitof(int n){
	return (int)(ceil(log(n)/log(2)));
}

static node *createTree(int size){
    node *tree = malloc(size * sizeof(node));
    return tree;
}

static void destroyTree(node *tree){
    free(tree);
}

static void update_offset(node *tree, int n, int max){
    for (int i = 0; i < max; i++){
        if (tree[i].offset != -1){
            tree[i].offset -= n;
        }
    }
}

static searcher find(node *tree, int root, unsigned char *window, int index, int size){
    int i, j;
    searcher off_len;
    /* initialization */
    off_len.offset = 0;
    off_len.length = 0;

    if (root == -1){
        return off_len;
    }
    j = root;
    /* flow the tree finding the longest match node */
    while (1){

        /* look for how many characters are equal between  the lookhead and the node */
        for (i = 0; window[index+i] == window[tree[j].offset + i] && i < size - 1; i++){}

        /* if the new match is better than the previous one, save the values */
        if (i > off_len.length){
            off_len.offset = index - tree[j].offset;
            off_len.length = i;
        }
        if (window[index+i] < window[tree[j].offset + i] && tree[j].left != -1){
            j = tree[j].left;
        }
        else if (window[index+i] > window[tree[j].offset + i ] && tree[j].right != -1){
            j = tree[j].right;
        }
        else break;
    }

    return off_len;
}

static void writecode(token tok, bitFILE *out, int la_size, int sb_size){
    bitIO_write(out, &tok.offset, bitof(sb_size));
    bitIO_write(out, &tok.length, bitof(la_size));
    bitIO_write(out, &tok.next, 8);
}

static token match(node *tree, int root, unsigned char *window, int la, int la_size){
    token tok;
    searcher best_sequentence;

    best_sequentence = find(tree, root, window, la, la_size);

    /* create the toke */
    tok.offset = best_sequentence.offset;
    tok.length = best_sequentence.length;
    tok.next = window[la+best_sequentence.length];
    return tok;
}

static void insert(node *tree, int *root, unsigned char *window, int abs_off, int len, int max){
    int i, tmp;
    int off = abs_off % max;
    if (*root != -1){
        i = *root;
        while(1){
            tmp = i;
            if (memcmp(&(window[abs_off]), &(window[tree[i].offset]), len) < 0){
                /* go to the left child */
                i = tree[i].left;
                if (i == -1){
                    /* set parent-child relation */
                    tree[tmp].left = off;
                    tree[off].parent = tmp;
                    break;
                }
            }
            else{
                /* go to the right child */
                i = tree[i].right;
                if(i == -1){
                    /* set parent-child relation */
                    tree[tmp].right = off;
                    tree[off].parent = tmp;
                    break;
                }
            }
        }        
    }
    else{
        *root = off;
        tree[*root].parent = -1;
    }
    /* set other parameters */
    tree[off].offset = abs_off;
    tree[off].length = len;
    tree[off].left = -1;
    tree[off].right = -1;
}

static int minChild(node *tree, int index){
    int min = index;
    while (tree[min].left != -1){
        min = tree[min].left;
    }
    return min;
}

static void delete(node *tree, int *root, unsigned char *window, int abs_sb, int max){
    int parent, child, sb;
    sb = abs_sb % max;
    if (tree[sb].left == -1){
        child = tree[sb].right;
        if (child != -1){
            tree[child].parent = tree[sb].parent;
        }
        parent = tree[sb].parent;
    }
    else if (tree[sb].right == -1){
        child = tree[sb].left;
        tree[child].parent = tree[sb].parent;
        parent = tree[sb].parent;       
    }
    else{
        child = minChild(tree, tree[sb].right);
        if (tree[child].parent == sb){
            parent = tree[sb].parent;
            tree[child].parent = parent;
        }
        else{
            parent = tree[child].parent;
            tree[parent].left = tree[child].right;
            if (tree[child].right != -1){
                tree[tree[child].right].parent = parent;
            }
            tree[child].right = tree[sb].right;
            tree[child].parent = tree[sb].parent;
            if (tree[child].right != -1){
                tree[tree[child].right].parent = child;
            }
            parent = tree[child].parent;
        }
        tree[child].left = tree[sb].left;
        if (tree[child].left != -1){
            tree[tree[child].left].parent = child;
        }
    }

    if (parent != -1){
        if (tree[parent].right == sb){
            tree[parent].right = child;
        }
        else{
            tree[parent].left = child;
        }
    }
    else {
        *root = child;
    }
    
}

void encode(FILE *file, bitFILE *out){
    int root = -1;
    int eof;
    node *tree;
    token tok;
    unsigned char *window;
    int la_size, sb_size = 0;
    int buff_size;
    int sb_index = 0, la_index = 0;
    int WINDOW_SIZE, SB_SIZE_INFO, LA_SIZE_INFO;
    SB_SIZE_INFO = SB_SIZE;
    LA_SIZE_INFO = LA_SIZE;

    /* Set window parameters */
    WINDOW_SIZE = (SB_SIZE * N) + LA_SIZE;

    window = calloc(WINDOW_SIZE, sizeof(unsigned char));

    tree = createTree(SB_SIZE);

    /* write header */
    bitIO_write(out, &SB_SIZE_INFO, MAX_BIT_BUFFER);
    bitIO_write(out, &LA_SIZE_INFO, MAX_BIT_BUFFER);

    /* fill the lookhead with the first LA_SIZE bytes or until EOF is reached */
    buff_size = fread(window, 1, WINDOW_SIZE, file);
    if(ferror(file)){
        printf("Error loading the data in the window.\n");
        return;
    }

    eof = feof(file);

    la_size = (buff_size > LA_SIZE) ? LA_SIZE : buff_size;

    while(buff_size > 0){

        /* find the longest match of the lookhead in the tree */
        tok = match(tree, root, window, la_index, la_size);

        /* write the token in the output file */
        writecode(tok, out, LA_SIZE, SB_SIZE);

        /* read as many bytes as matched in the previous iteration*/
        for (int i = 0; i < tok.length + 1; i++){
            /* if search buffer's length is max, the oldest node is removed from the tree */
            if (sb_size == SB_SIZE){
                delete(tree, &root, window, sb_index, SB_SIZE);
                sb_index++;
            }
            else{
                sb_size++;
            }

            /* insert a new node in the tree */
            insert(tree, &root, window, la_index, la_size, SB_SIZE);
            la_index++;

            if(eof == 0){
                /* scroll backward the buffer when it is almost full */
                if (sb_index != SB_SIZE * (N - 1)){
                    continue;
                }
                memmove(window, &(window[sb_index]), sb_size + la_size);

                /* update the node's offset when the buffer is scrolled */
                update_offset(tree, sb_index, SB_SIZE);

                sb_index = 0;
                la_index = sb_size;

                /* read from file */

                buff_size += fread(&(window[sb_size+la_size]), 1, WINDOW_SIZE - (sb_size + la_size), file);
                if (ferror(file)){
                    printf("Error loading the data in window.\n");
                    return;
                }
                eof = feof(file);
                
            }
            buff_size--;
            /* case where we hit EOF before filling lookhead */
            la_size = (buff_size > LA_SIZE) ? LA_SIZE : buff_size;
        }
    }

    destroyTree(tree);
    free(window);
}

bitFILE *bitIO_open(const char *path){
    bitFILE *bitF;

    /* inintialize structure */
    bitF = (bitFILE*)malloc(sizeof(bitFILE));
    bitF->bytepos = 0;
    bitF->bitpos = 0;
    bitF->buffer = (unsigned char*)malloc(BIT_IO_BUFFER * sizeof(unsigned char));
    memset(bitF->buffer, 0, BIT_IO_BUFFER); // fill buffer as zeros (for inizialisation)

    // open file
    if ((bitF->file = fopen(path, "w")) == NULL){
        return NULL;
    }

    return bitF;
}

int bitIO_close(bitFILE *bitF){

    /* errors handler */
    if(bitF == NULL || bitF->file == NULL){
        return -1;
    }

    /* write the unwritten bytes into the file */
    if (bitF->bitpos > 0){
            bitF->bytepos++;
    }
    write_buffer(bitF);

    /* close the file */
    fclose(bitF->file);
    /* free memory */
    free(bitF->buffer);
    free(bitF);

    return EXIT_SUCCESS;
}