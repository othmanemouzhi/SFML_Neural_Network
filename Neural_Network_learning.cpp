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

int maximum(vector <unsigned> topology){
    int m = topology[0];
    for(int i=0;i<topology.size();i++){
        if(m<topology[i]){
            m=topology[i];
        }
    }
    return m;
}

int main(){
    TrainingData trainData("TrainingData.txt");

    // e.g., { 3, 2, 1 }
    vector<unsigned> topology;
    trainData.getTopology(topology);

    vector<double> inputVals, targetVals, resultVals;
    int trainingPass = 0;

    // vector<unsigned> topology = {2,4,1};
    Net myNetwork(topology);

    sf::RenderWindow window(sf::VideoMode(800,550),"Neural Networks",sf::Style::Default);
    window.setFramerateLimit(60);
    //window.setVerticalSyncEnabled(true);
    // changement de la position de la fenêtre (relativement au bureau)
    //window.setPosition(sf::Vector2i(10,50));
    
    int number(maximum(topology));
    CircleShape objet(window.getSize().y/(5*number));
    objet.setFillColor(Color(127, 221, 76));

    vector<CircleShape> layer;
    // layer.push_back(objet);
    // Définition du réseaux 
    vector<vector<CircleShape>> Reseaux;
    cout << topology.size() << endl;
    for(int i=0;i<topology.size();i++){
        Reseaux.push_back(layer);
        int a  = (int)((window.getSize().y-topology[i]*objet.getRadius())/(topology[i]+1));
        int b  = (int)((window.getSize().x-topology.size()*2*objet.getRadius())/(2*topology.size()));
        for(int j=0;j<topology[i];j++){
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
    int time = 0;
    int iteration = 20;

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
        if(time==iteration){
            // Update
            if (!trainData.isEof()) {
            ++trainingPass;
            cout << endl << "Pass " << trainingPass;

            // Get new input data and feed it forward:
            
            if (trainData.getNextInputs(inputVals) != topology[0]) {
                break;
            }
            
            
            showVectorVals(": Inputs:", inputVals);
            myNetwork.feedForward(inputVals);

            // Collect the net's actual output results:
            myNetwork.getResults(resultVals);
            showVectorVals("Outputs:", resultVals);

            // Train the net what the outputs should have been:
            trainData.getTargetOutputs(targetVals);
            showVectorVals("Targets:", targetVals);
            assert(targetVals.size() == topology.back());

            myNetwork.backProp(targetVals);

            // Report how well the training is working, average over recent samples:
            // cout << "Net recent average error: "  << myNet.getRecentAverageError() << endl;
            }
            time = 0;
        }
        else{
            time++;
        }

        // Draw
        
        window.clear(Color(96, 80, 220,255));
        window.draw(BackGround);
        for(int i=0;i<topology.size();i++){
            for(int j=0;j<topology[i];j++){
                window.draw(Reseaux[i][j]);
                std::stringstream ss;
                ss << (myNetwork.get_layers()[i][j]).get_outputVal();
                mytext.setString(ss.str());
                mytext.setPosition(Reseaux[i][j].getPosition().x+objet.getRadius()*3/4,Reseaux[i][j].getPosition().y+objet.getRadius()/5);
                // Draw the text
                window.draw(mytext);
            }
        }
        for(int i=0;i<topology.size()-1;i++){
            for(int j=0;j<topology[i];j++){
                for(int k=0;k<topology[i+1];k++){
                    sf::Vertex line[] =
                    {
                        sf::Vertex(sf::Vector2f(Reseaux[i][j].getPosition().x+2*Reseaux[i][j].getRadius(),Reseaux[i][j].getPosition().y+Reseaux[i][j].getRadius() )),
                        sf::Vertex(sf::Vector2f(Reseaux[i+1][k].getPosition().x,Reseaux[i+1][k].getPosition().y+Reseaux[i+1][k].getRadius() ))
                    };
                    window.draw(line, 2, sf::Lines);
                    std::stringstream ss;
                    ss << (myNetwork.get_layers()[i][j]).get_weight(k);
                    mytext.setString(ss.str());
                    mytext.setPosition((Reseaux[i][j].getPosition().x+2*Reseaux[i][j].getRadius()+Reseaux[i+1][k].getPosition().x)/2,(Reseaux[i][j].getPosition().y+Reseaux[i][j].getRadius()+Reseaux[i+1][k].getPosition().y+Reseaux[i+1][k].getRadius())/2-3/2*objet.getRadius());
                    // Draw the text
                    window.draw(mytext);
                }
            }
        }

        std::stringstream ss;
        ss << "OR";
        mytext.setString(ss.str());
        mytext.setCharacterSize(50);
        mytext.setPosition(Reseaux[0][0].getPosition().x,(Reseaux[0][1].getPosition().y+Reseaux[0][0].getPosition().y)/2);
        window.draw(mytext);
        mytext.setCharacterSize(30);
        // window.draw(line, 2, sf::Lines);

        // fin de la frame courante, affichage de tout ce qu'on a dessiné
        window.display();
        }

    return 0;
}


