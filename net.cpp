#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include "Neuron.hpp"
#include <sstream>
#define PI 3.14159265

// g++ net.cpp Neuron.cpp -o net.exe -lsfml-graphics -lsfml-window -lsfml-system

using namespace sf;

int maximum(vector <unsigned> top){
    int m = top[0];
    for(int i=0;i<top.size();i++){
        if(m<top[i]){
            m=top[i];
        }
    }
    return m;
}

int main(){
    vector<unsigned> top = {2,4,1};
    Net myNetwork(top);

    sf::RenderWindow window(sf::VideoMode(800,550),"Neural Networks",sf::Style::Default);
    window.setFramerateLimit(60);
    //window.setVerticalSyncEnabled(true);
    // changement de la position de la fenêtre (relativement au bureau)
    //window.setPosition(sf::Vector2i(10,50));
    
    int number(maximum(top));
    CircleShape objet(window.getSize().y/(5*number));
    objet.setFillColor(Color(127, 221, 76));

    vector<CircleShape> layer;
    // layer.push_back(objet);
    // Définition du réseaux 
    vector<vector<CircleShape>> Reseaux;
    cout << top.size() << endl;
    for(int i=0;i<top.size();i++){
        Reseaux.push_back(layer);
        int a  = (int)((window.getSize().y-top[i]*objet.getRadius())/(top[i]+1));
        int b  = (int)((window.getSize().x-top.size()*2*objet.getRadius())/(2*top.size()));
        for(int j=0;j<top[i];j++){
            objet.setPosition(b + (2*objet.getRadius()+2*b)*i,a +(a+objet.getRadius())* j);
            Reseaux[i].push_back(objet);
        }
        cout << Reseaux[i].size() <<endl;
    }

    Texture t1;
    t1.loadFromFile("neural_network.png");

    Sprite BackGround(t1);

    /*

    sf::CircleShape shape(50);

    // colle un rectangle texturé de 100x100 sur la forme
    shape.setTexture(&texture); // texture est un sf::Texture
    shape.setTextureRect(sf::IntRect(10, 10, 100, 100));

    */

    clock_t temps;
    bool Pause=false;

    sf::Text mytext;
    mytext.setPosition(50,50);
    mytext.setFillColor(Color::White);
    sf::Font font;
    if (!font.loadFromFile("starmoon/Starmoon.ttf"))
    {
        std::cerr << "error";
    }
    mytext.setFont(font);

    while(window.isOpen()){

        sf::Event event;
        while(window.pollEvent(event) || Pause){

            if(event.type==sf::Event::Closed){
                window.close();
            }
            if(event.type==Event::KeyPressed && event.key.code == sf::Keyboard::Space){
                if(Pause){
                    Pause=false;
                }
                else{
                    Pause=true;
                }
            }
        }

        // Update
        

        // Draw
        
        window.clear(Color(96, 80, 220,255));
        window.draw(BackGround);
        for(int i=0;i<top.size();i++){
            for(int j=0;j<top[i];j++){
                window.draw(Reseaux[i][j]);
                std::stringstream ss;
                ss << (myNetwork.get_layers()[i][j]).get_outputVal();
                mytext.setString(ss.str());
                mytext.setPosition(Reseaux[i][j].getPosition().x+objet.getRadius()*3/4,Reseaux[i][j].getPosition().y+objet.getRadius()/5);
                // Draw the text
                window.draw(mytext);
            }
        }
        for(int i=0;i<top.size()-1;i++){
            for(int j=0;j<top[i];j++){
                for(int k=0;k<top[i+1];k++){
                    sf::Vertex line[] =
                    {
                        sf::Vertex(sf::Vector2f(Reseaux[i][j].getPosition().x+2*Reseaux[i][j].getRadius(),Reseaux[i][j].getPosition().y+Reseaux[i][j].getRadius() )),
                        sf::Vertex(sf::Vector2f(Reseaux[i+1][k].getPosition().x,Reseaux[i+1][k].getPosition().y+Reseaux[i+1][k].getRadius() ))
                    };
                    window.draw(line, 2, sf::Lines);
                    std::stringstream ss;
                    ss << (myNetwork.get_layers()[i][j]).get_outputVal();
                    mytext.setString(ss.str());
                    mytext.setPosition((Reseaux[i][j].getPosition().x+2*Reseaux[i][j].getRadius()+Reseaux[i+1][k].getPosition().x)/2,(Reseaux[i][j].getPosition().y+Reseaux[i][j].getRadius()+Reseaux[i+1][k].getPosition().y+Reseaux[i+1][k].getRadius())/2-3/2*objet.getRadius());
                    // Draw the text
                    window.draw(mytext);
                }
            }
        }

        // window.draw(line, 2, sf::Lines);

        // fin de la frame courante, affichage de tout ce qu'on a dessiné
        window.display();
        }

    return 0;
}


