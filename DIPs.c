#include "DIPs.h"

/*black and white conversion*/
void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
    int             x, y, tmp;

    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            tmp = (R[x][y] + G[x][y] + B[x][y]) / 3;
            R[x][y] = G[x][y] = B[x][y] = tmp;
        }
    }
}

/* edge detection */
void Edge(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
    int             x, y, m, n, a, b;
    unsigned char   tmpR[WIDTH][HEIGHT];
    unsigned char   tmpG[WIDTH][HEIGHT];
    unsigned char   tmpB[WIDTH][HEIGHT];
    for (y = 0; y < HEIGHT; y++){
        for (x = 0; x < WIDTH; x++) {
            tmpR[x][y] = R[x][y];
            tmpG[x][y] = G[x][y];
            tmpB[x][y] = B[x][y];
        }
    }
    int sumR = 0;   /* sum of the intensity differences with neighbors */
    int sumG = 0;
    int sumB = 0;
    for (y = 1; y < HEIGHT - 1; y++){
        for (x = 1; x < WIDTH - 1; x++){
            for (n = -1; n <= 1; n++){
                for (m = -1; m <= 1; m++) {
                    a = (x + m >= WIDTH) ? WIDTH - 1 : (x + m < 0) ? 0 : x + m;
                    b = (y + n >= HEIGHT) ? HEIGHT - 1 : (y + n < 0) ? 0 : y + n;
                    sumR += (tmpR[x][y] - tmpR[a][b]);
                    sumG += (tmpG[x][y] - tmpG[a][b]);
                    sumB += (tmpB[x][y] - tmpB[a][b]);
                }
            }
            R[x][y] = (sumR > MAX_PIXEL) ? MAX_PIXEL: (sumR < 0) ? 0: sumR;
            G[x][y] = (sumG > MAX_PIXEL) ? MAX_PIXEL: (sumG < 0) ? 0: sumG;
            B[x][y] = (sumB > MAX_PIXEL) ? MAX_PIXEL: (sumB < 0) ? 0: sumB;
            sumR = sumG = sumB = 0;
        }
    }
 /* set all four borders to 0 */
    for (y = 0; y < HEIGHT; y++) {
        R[0][y] = 0;
        G[0][y] = 0;
        B[0][y] = 0;
        R[WIDTH - 1][y] = 0;
        G[WIDTH - 1][y] = 0;
        B[WIDTH - 1][y] = 0;
    }
    for (x = 0; x < WIDTH; x++) {
        R[x][0] = 0;
        G[x][0] = 0;
        B[x][0] = 0;
        R[x][HEIGHT - 1] = 0;
        G[x][HEIGHT - 1] = 0;
        B[x][HEIGHT - 1] = 0;
    }
}

/* Shuffle the image */
void Shuffle(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
    int block_cnt = SHUFF_HEIGHT_DIV * SHUFF_WIDTH_DIV;
    int block_width = WIDTH/SHUFF_WIDTH_DIV;
    int block_height = HEIGHT/SHUFF_HEIGHT_DIV;
    int block[SHUFF_WIDTH_DIV][SHUFF_HEIGHT_DIV];
    int i, j;


    srand(time(NULL));
    /* Initialize block markers to not done (-1) */
    for (i = 0; i < SHUFF_WIDTH_DIV; i++) {
        for (j = 0; j< SHUFF_HEIGHT_DIV; j++) {
            block[i][j] = -1;
        }
    }

    while (block_cnt > 0) {
        /* Generate a random pair of blocks */
        int dest_height = rand() % SHUFF_HEIGHT_DIV;
        int dest_width = rand() % SHUFF_WIDTH_DIV;
        int src_height = rand() % SHUFF_HEIGHT_DIV;
        int src_width = rand() % SHUFF_WIDTH_DIV;
 /* Check if these blocks are already swaped, if not swap blocks */
        if ((block[dest_width][dest_height] == -1) && (block[src_width][src_height] == -1)) {
            /* Swap blocks */
            for (i = 0; i < block_height; i++) {
                /* Init src and dest height offset */
                int h_dest = ((dest_height * block_height) + i) % HEIGHT;
                int h_src  = ((src_height * block_height) + i) % HEIGHT;
                for (j = 0; j < block_width; j++) {
                    int temp;
                    /* Init src and dest width offset */
                    int w_src  = ((src_width * block_width) + j) % WIDTH;
                    int w_dest = ((dest_width * block_width) + j) % WIDTH;

                    temp = R[w_dest][h_dest];
                    R[w_dest][h_dest] = R[w_src][h_src];
                    R[w_src][h_src] = temp;

                    temp = G[w_dest][h_dest];
                    G[w_dest][h_dest] = G[w_src][h_src];
                    G[w_src][h_src] = temp;

                    temp = B[w_dest][h_dest];
                    B[w_dest][h_dest] = B[w_src][h_src];
                    B[w_src][h_src] = temp;
                }
            }
            /* Set marker as done */
            block[dest_width][dest_height] = 1;
            block[src_width][src_height] = 1;
            /* Decrease block count */
            block_cnt -= 2; /* Two blocks are swapped */
        }

    }
}

