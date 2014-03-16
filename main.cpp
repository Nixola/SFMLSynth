#include <iostream>
#include <stdio.h>

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <stdlib.h>
#include <math.h>

#define BUFFSIZE 1536
#define twoto15 32768
#define TAU 6.28
#define RATE 44100
#define halftone 1.0594630943593

//using namespace std;

float freq;

int duration;

bool generate = true;
bool done = false;
int threadFunc();
sf::Thread thread(&threadFunc);

sf::Int16 Buffer[BUFFSIZE];



class Sine : public sf::SoundStream {

    public:
        float freq;
        int samplesN;

    private:
        virtual bool onGetData(Chunk& Data) {

            Data.samples = Buffer;
            Data.sampleCount = BUFFSIZE;
            generate = true;
            
            return true;

        }

        virtual void onSeek(sf::Time position) {

            samplesN = (int) position.asSeconds()*RATE;

            return;

        }


    public:
        void init() {
            unsigned int chan = 1;
            initialize(chan, RATE);
        }

};

Sine sine;

int main(int argC, char** argV) {

    int tim;
    tim = 0;

    if (argC >= 2)
        freq = (float) atoi(argV[1]);
    else
        freq = 48;

    if (argC >= 3)
        tim = atoi(argV[2]);

    duration = (int) (double) tim * RATE/1000;

    freq = pow(halftone, freq-48.0)*440.0;

    sine.freq = freq;
    sine.samplesN = 0;

    thread.launch();

    sf::sleep(sf::milliseconds(10));

    sine.init();

    sine.play();

    if (tim == 0)
        std::cin >> freq;
    else
        sf::sleep(sf::milliseconds(tim));

    done = true;
    return 0;

}

int threadFunc() {

    while (!done) {

        if (generate) {

            int i;
            double m;
            m = freq/RATE*TAU;
            for (i = 0; i < BUFFSIZE; i++) {
                sine.samplesN++;
                //Buffer[i] = (short int) ((sin(sine.samplesN*m)+sin(sine.samplesN*m/2)+sin(sine.samplesN*m*2)+sin(sine.samplesN*m*4))/4*twoto15);
                Buffer[i] = (short int) (sin(sine.samplesN*m)*twoto15);
            }

            generate = false;

        }

        sf::sleep(sf::milliseconds(10));

    }

    return 0;

}