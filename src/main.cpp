#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <chrono>
#include <thread>


class World{
    private:
    static const int width = 161, height = 161;

    int private_grid[width][height];
    int grid[width][height];
    
    public:
    const float sandSize = 5;

    World(){
        for(int i = 0; i < width-1; i++)
            for (int j = 0; j < height-1; j++){
                private_grid[i][j] = 0;
                grid[i][j] = 0;
            }
    }
    void update(){
        for(int i = 0; i < width-1; i++){
            for(int j = 0; j < height - 1; j++){
            if(private_grid[i][j] == 1){
                    
                if(private_grid[i][j+1] == 0){
                        grid[i][j] = 0;
                        grid[i][j+1] = 1;
                }
                else if(private_grid[i+1][j+1] == 0 && private_grid[i-1][j+1] == 0){
                    bool direction = rand() % 2; 
                    if(direction == 0 ){
                        grid[i][j] =0;
                        grid[i-1][j+1] = 1;
                    }
                    else{
                        grid[i][j] = 0;
                        grid[i+1][j+1] = 1;
                    }
                } 
                else if(private_grid[i+1][j+1] == 0){
                    grid[i][j] =0;
                    private_grid[i+1][j+1] = 1;
                }
                else if(private_grid[i-1][j+1] ==0){
                    grid[i][j] = 0;
                    private_grid[i-1][j+1] = 1;
                }
                }
            }
            }
        }

    void draw(sf::RenderWindow& window){
        for(int i = 0; i < width-1; i++){
            for(int j = 0; j < height - 1; j++){
                if(grid[i][j] == 1){
                    sf::RectangleShape shape = sf::RectangleShape();

                    shape.setSize(sf::Vector2f(sandSize, sandSize));
                    shape.setFillColor(sf::Color::Yellow);
                    shape.setPosition({i * sandSize, j * sandSize});
                    window.draw(shape); // assuming 'window' is a valid sf::RenderWindow instance
                    }
                    private_grid[i][j] = grid[i][j]; // copy current state to private grid
                }
            }
        }
    

    void placeSand(int x, int y){
        grid[x][y] = 1;
    }

    void reset(){ //clearing window
        for(int i = 0; i < width-1; i++)
            for (int j = 0; j < height-1; j++){
                private_grid[i][j] = 0;
                grid[i][j] = 0;
            }
    }
};


int main()
{

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Falling Sand Simulation", sf::Style::Close);

    window.setFramerateLimit(60);

    const float updateRate = 0.01f; // adjust to change the rate at which the sand falls

    float countdownMS = updateRate; // used to time the update of the world
	float toggleCounterMS = 0.0f;
	float toggleThresholdMS = 0.125f; // used to allow pausing/unpausing

    World world;

    sf:: Clock clock; // to keep track of time
    bool isPaused = false; // to keep track of whether the simulation is paused


    // font
    sf::Font font;
    if (!font.openFromFile("/Users/kacperolszewski/Desktop/Visual Studio Code projekty/falling sand 2/src/arial.ttf"))
    {
       std:: cout<< "Error loading font" << std ::endl;
        return -1; // exit if font loading fails
    }

    sf::Text text(font);
    text.setString("Choose color");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition({10, 570}); // position the text in the window



    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        
        
        while(const std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                //place sand
                sf:: Vector2i localPosition = sf::Mouse::getPosition(window);
                int gridX = localPosition.x / world.sandSize;
                int gridY = localPosition.y / world.sandSize;

               world.placeSand(gridX, gridY);
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
                if(toggleCounterMS > toggleThresholdMS){
                    isPaused = !isPaused; // toggle pause state
                    toggleCounterMS = 0.0f; // reset counter
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)){
                world.reset(); // reset the world
            }
        }
        
        float sec = clock.restart().asSeconds();
        countdownMS -= sec;
        toggleCounterMS += sec;



        if(countdownMS < 0.0f){
            if(!isPaused){
                world.update(); // update the world only if not paused
                countdownMS = updateRate; // reset countdown
            }
        }

        world.draw(window); // draw the world
        window.draw(text); // draw the text
        window.display(); // display the contents of the window
    }

    
}