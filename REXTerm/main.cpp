#include <iostream>
#include "REXReader++/RexReader.h"
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <map>
#include <math.h>
#include <algorithm>
#include <tuple>

using namespace std;

void printOptions()
{
    cout<< "Usage:" << endl;
    cout << "\t-i: Load a .xp file" << endl;
    cout << "\t-s: Set the time interval between frames (in milliseconds)" << endl;
    cout << "\t-w: Disable color output" << endl;
    cout << "\t-o: Play the animation only once without looping" << endl;
    cout << "\t-d: Print debug information for color pairs" << endl;
    cout << "\t-h: Print this output" << endl;
}

    struct REXRGB{
               int R;
               int G;
               int B;

               bool operator==(const REXRGB &o) const {
                       return R == o.R && G == o.G && B == o.B;
                   }

               bool operator!=(const REXRGB &o) const {
                       return R != o.R && G != o.G && B != o.B;
                   }

               bool operator<(const REXRGB &o)  const {
                   return tie(R, G, B) < tie(o.R, o.G, o.B);
                   }
           };

    struct RGBPair{
               REXRGB glyph;
               REXRGB back;

               bool operator==(const RGBPair &o) const {
                       return glyph == o.glyph && back == o.back;
                   }

               bool operator!=(const RGBPair &o) const {
                       return glyph != o.glyph && back != o.back;
                   }

               bool operator<(const RGBPair &o)  const {
                   return tie(glyph, back) < tie(o.glyph, o.back);
                   }
           };

int main(int argc, char* argv[])
{
    /* BETTER COMMAND LINE FLAG MANAGEMENT UNDER CONSTRUCTION
     * IDEAS:
     *      -Playlist: give xpReader a vector of.xp files to switch on the fly to create longer animations
     *      -Color: activate/deactivate color output
     *      -Center: center the animation in the console
     */




    char c=0;

    bool stop = false;
    bool color = true;
    bool loop = true;
    bool debug = false;

    short currentColor=1;
    short currentPair = 2;

    int speed = 170;

    RexReader xpReader;

    REXRGB gRGB;
    REXRGB bRGB;
    REXRGB Transparent;
    RGBPair pair;
    RGBPair TransparentPair;

    map<REXRGB,int> colorPalette;
    map<RGBPair,int> pairList;

    vector<char*> input;

    int opt;

    while ((opt = getopt(argc, argv, "i:s:wohd")) != -1) {
            switch (opt) {
            case 'i':
                input.push_back(optarg);
                break;
            case 's':
                speed= atoi(optarg);
                break;
            case 'w':
                color=false;
                break;
            case 'o':
                loop=false;
                break;
            case 'd':
                debug=true;
                break;
            case 'h':
                printOptions();
                return 0;
            default: /* '?' */
                cout << "\tUsage: -i file.xp" << endl;
                exit(EXIT_FAILURE);
            }
        }

    if(input.size()==0)
    {
        cout << "\tUsage: -i file.xp" << endl;
        exit(EXIT_FAILURE);
    }



    initscr();
    cbreak();
    noecho();
    curs_set(0);
    if(color==true)
    start_color();

    Transparent.R = 0;
    Transparent.G = 0;
    Transparent.B = 0;

    colorPalette[Transparent] = 0;

    Transparent.R = 255;
    Transparent.G = 0;
    Transparent.B = 255;

    colorPalette[Transparent] = 0;

    TransparentPair.glyph=Transparent;
    TransparentPair.back=Transparent;

    pairList[TransparentPair]=1;
    xpReader.LoadFile(input[0]);
    RexTileMap* tilemap = xpReader.GetTileMap();
    if(color==true)
    {
        for(int l=0; l<xpReader.GetLayerCount() && !stop; l++)
        {
            for(int i=0; i< (xpReader.GetLayerWidth())*(xpReader.GetLayerHeight()); i++)
            {
                gRGB.R = tilemap->Layers[l]->Tiles[i]->ForegroundRed;
                gRGB.G = tilemap->Layers[l]->Tiles[i]->ForegroundGreen;
                gRGB.B = tilemap->Layers[l]->Tiles[i]->ForegroundBlue;

                if(colorPalette.find(gRGB)==colorPalette.end() && currentColor<=255)
                {

                    init_color(currentColor, round(gRGB.R*3.9215), round(gRGB.G*3.9215), round(gRGB.B*3.9215));
                    colorPalette[gRGB]=currentColor;
                    currentColor++;
                }

                bRGB.R = tilemap->Layers[l]->Tiles[i]->BackgroundRed;
                bRGB.G = tilemap->Layers[l]->Tiles[i]->BackgroundGreen;
                bRGB.B = tilemap->Layers[l]->Tiles[i]->BackgroundBlue;

                if(colorPalette.find(bRGB)==colorPalette.end() && currentColor<=255)
                {
                    init_color(currentColor, round(bRGB.R*3.9215), round(bRGB.G*3.9215), round(bRGB.B*3.9215));
                    colorPalette[bRGB]=currentColor;
                    currentColor++;
                }

                pair.glyph=gRGB;
                pair.back =bRGB;

                if(pairList.find(pair)==pairList.end())
                {
                    init_pair(currentPair, colorPalette.find(gRGB)->second, colorPalette.find(bRGB)->second);
                    pairList[pair]=currentPair;
                    currentPair++;
                }
            }
        }
    }

    for(int l=0; l<xpReader.GetLayerCount() && !stop; l++)
    {
        for(int i=0, z=0; i< xpReader.GetLayerWidth()*(xpReader.GetLayerHeight()); i++)
        {

            gRGB.R = tilemap->Layers[l]->Tiles[i]->ForegroundRed;
            gRGB.G = tilemap->Layers[l]->Tiles[i]->ForegroundGreen;
            gRGB.B = tilemap->Layers[l]->Tiles[i]->ForegroundBlue;

            bRGB.R = tilemap->Layers[l]->Tiles[i]->BackgroundRed;
            bRGB.G = tilemap->Layers[l]->Tiles[i]->BackgroundGreen;
            bRGB.B = tilemap->Layers[l]->Tiles[i]->BackgroundBlue;

            pair.glyph=gRGB;
            pair.back =bRGB;

            if(color==true)
            attron(COLOR_PAIR(pairList[pair]));
            mvaddch(z,i%xpReader.GetLayerWidth(),tilemap->Layers[l]->Tiles[i]->CharacterCode);
            //move(i/xpReader.GetLayerHeight(),i%xpReader.GetLayerWidth());
            //printw("%d",pairList[pair];

            if(i%xpReader.GetLayerWidth()==xpReader.GetLayerWidth()-1)
            {
                z++;
            }

            if(color==true)
            attroff(COLOR_PAIR(pairList[pair]));
        }
        refresh();
        timeout(speed);
        c = getch();
        if(c=='q')
        {
            stop=true;
        }
        if(l==xpReader.GetLayerCount()-1 && loop==true)
        {
            l=-1;
        }
    }
    endwin();

    if(debug)
    {
        cout<<"COLORS LIST"<<endl;
        cout<<"CURRENT COLOR: "<<currentColor -1 << endl;
        cout<<"COLOR LIST SIZE"<< colorPalette.size() << endl;

        for(map<REXRGB,int>::iterator it = colorPalette.begin(); it!=colorPalette.end(); it++)
        {
            cout<<"R: "<<it->first.R<<" G: " <<it->first.G<< " B: "<<it->first.B << " ID: " << it->second << endl;
        }

        cout<<"PAIRS LIST"<<endl;
        cout<<"CURRENT PAIR: "<<currentPair-1<< endl;
        cout<<"PAIR LIST SIZE"<< pairList.size() << endl;

        for(map<RGBPair,int>::iterator it = pairList.begin(); it!=pairList.end(); it++)
        {
            cout<< "FRONT [" << colorPalette[it->first.glyph]<< "] BACK ["<< colorPalette[it->first.back] << "] ID: " << it->second << endl;
        }
    }

}
