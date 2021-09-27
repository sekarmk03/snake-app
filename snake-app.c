#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys\timeb.h>
#include<windows.h>

/*---------struktur-----------*/

/* struktur untuk menampung tiap segment dari snake (array) */
struct Segment {
    int x, y;
};

/*--------variabel global-------*/

//array untuk menampung data ular
struct Segment snake[2000];

//variabel untuk menyimpang panjang ular
int panjang = 0;

//kecepatan gerakan ular dalam ms
int snake_speed = 200;

//arah kepala saat awal mulai permainan
int dir = VK_RIGHT;

//panjang console
int console_width = 60;

//tinggi console
int console_height = 25;

//posisi makanan
int food_x, food_y;


/*-------fungsi-fungsi----------*/

//push segment ke snake (pada bagian head)
void push(int x, int y){
    for (int i = panjang; i > 0; i--)
    {
        snake[i] = snake[i-1];
    }
    snake[0].x = x;
    snake[0].y = y;
    panjang++;
}

//pop bagian ekor snake
void pop(){
    panjang--;
}

/*-------konfigurasi permainan-------*/

//panjang segment snake saat awal permainan
int snake_size = 3;


/*
    pindahkan posisi kursor di layar
    fungsi ini spesifik untuk os windows */

void gotoxy(int x, int y){
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/*
    gambar snake di layar */
void display(){
    for (int i = 0; i < panjang; i++)
    {
        //cetak di posisi x,y
        gotoxy(snake[i].x, snake[i].y);
        printf("O");
    }
}

int check_collision(int x, int y){
    /* memeriksa apakah terdapat salah satu segment snake (array) di koordinat x,y.
        return 0 artinya tidak bertumpuk, 1 artinya bertumpuk */
    for (int i = 0; i < panjang; i++)
    {
        if (snake[i].x == x && snake[i].y == y)
        {
            return 1;
        }
    }
    return 0;
}

void place_food(){
    /* taruh makanan secara acak, namun memastikan makanan tidak
    bertumpuk dengan salah satu segmen snake */
    //jika makanan bertumpuk dengan salah satu segmen snake,
    //ulangi penempatan makanan secara acak
    do
    {
        food_x = rand() % console_width;
        food_y = rand() % console_height;
    } while (check_collision(food_x, food_y) == 1);
    
}


/*
    program utama
    */
int main(){
    //randomize
    srand(time(NULL));

    //untuk menyimpan penanda waktu
    struct timeb last_timestamp;
    ftime(&last_timestamp); //set nilai awal

    //untuk menyimpan nilai
    int score = 0;
    
    //push segment ke kanan
    //sebanyak 3 segment
    for (int i = 0; i < snake_size; i++)
    {
        push(i, 0);
    }

    //tempatkan makanan secara acak
    place_food();

    //game loop
    int i = 0;
    while (i >= 0)
    {
        //ambil penanda waktu saat ini
        struct timeb current_timestamp;
        ftime(&current_timestamp);

        //selisih waktu terakhir dengan waktu sekarang
        int interval = 1000 * (current_timestamp.time - last_timestamp.time) + (current_timestamp.millitm - last_timestamp.millitm);

        //snake bergerak setiap 200 ms
        //dihitung dengan membandingkan selisih waktu sekarang dengan waktu terakhir snake bergerak
        if (interval >= snake_speed)
        {
            //tentukan posisi x,y ke mana snake akan bergerak
            //posisi dilihat dari koordinan segment kepala dan arah (variabel dir)
            int x, y;
            switch (dir)
            {
            case VK_LEFT:
                x = snake[0].x - 1;
                y = snake[0].y;
                break;
            case VK_RIGHT:
                x = snake[0].x + 1;
                y = snake[0].y;
                break;
            case VK_UP:
                x = snake[0].x;
                y = snake[0].y - 1;
                break;
            case VK_DOWN:
                x = snake[0].x;
                y = snake[0].y + 1;
                break;
            }

            //jika posisi head menabrak pembatas
            //maka permainan berakhir (keluar dari game loop)
            if (x < 0 || x >= console_width || y < 0 || y >= console_height)
            {
                break;
            }
            //jika posisi head menabrak dirinya sendiri
            //(posisi head sama dengan salah satu segmen)
            //maka permainan berakhir (keluar dari game loop)
            if (check_collision(x, y) == 1)
            {
                break;
            }

            //jika tidak terjadi tabrakan (collision), maka snake
            //boleh bergerak maju
            //pop ekor, lalu push segment ke depan head sehingga snake tampak bergerak maju
            //namun jika posisi x,y ke mana head snake akan bergerak berada di posisi
            //makanan, tidak perlu pop sehingga segmen bertambah panjang
            if (x == food_x && y == food_y)
            {
                //dalam kondisi ini snake memakan makanan, maka nilai bertambah
                score += 100;
                
                //lalu makanan ditempatkan ulang secara acak
                place_food();
            }
            else
            {
                pop();                
            }
            
            push(x, y);

            //tampilkan kondisi permainan saat ini di layar
            //bersihkan layar
            system("cls");

            //cetak snake di layar
            display();

            //cetak makanan di layar
            gotoxy(food_x, food_y);
            printf("X");

            //perbarui penanda waktu
            last_timestamp = current_timestamp;
        }

        //ubah arah jika tombol panah ditekan
        if (GetKeyState(VK_LEFT) < 0)
        {
            dir = VK_LEFT;
        }
        if (GetKeyState(VK_RIGHT) < 0)
        {
            dir = VK_RIGHT;
        }
        if (GetKeyState(VK_UP) < 0)
        {
            dir = VK_UP;
        }
        if (GetKeyState(VK_DOWN) < 0)
        {
            dir = VK_DOWN;
        }
        
        //keluar dari program jika menekan tombol ESC
        if (GetKeyState(VK_ESCAPE) < 0)
        {
            return 0;
        }
        i++;
    }
    
    //setelah keluar dari game loop, berarti permainan berakhir
    system("cls");
    printf("GAME OVER\n");
    printf("Your score : %d\n\n");
    printf("Press ENTER to exit..");
        
    getchar();
    return 0;
}