/// <summary>
/// author Pete Lowe May 2019
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum class Direction
{
	None,
	Left,
	Right
};

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void processMouseUp(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupFontAndText();
	void setupSprite();
	void setupAudio();

	void animateHelicopter();
	void moveHelo();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo
	bool m_exitGame; // control exiting game


	sf::Texture m_heloTexture;
	sf::Sprite m_heloSprite;
	sf::Vector2f m_heloLocation{ 100.0f,100.0f };
	sf::Vector2f m_heloVelocity{ 1.0f,1.0f }; // change in location speed & direction
	float m_speed = 5.23f; // how many pixels per clock tick

	Direction m_direction{ Direction::None };
	sf::Vector2f m_target{0.0f,0.0f}; // location of mouse click

	int m_frame{0};//current frame of helo animation
	float m_framecounter {0.0f};// frame counter
	float m_frameIncrement = 0.3f; // frame inc rate updated 60 fps


	sf::SoundBuffer m_heloSoundBuffer;
	sf::Sound m_heloSound;
	sf::Music m_music;

};

#endif // !GAME_HPP

