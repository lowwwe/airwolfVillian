/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupAudio();
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseButtonReleased == newEvent.type)
		{
			processMouseUp(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

void Game::processMouseUp(sf::Event t_event)
{
	float lenght = 0.0f;
	sf::Vector2f displacement;
	if (sf::Mouse::Left == t_event.mouseButton.button)
	{
		m_frameIncrement = 0.6f;
		displacement.x = static_cast<float>(t_event.mouseButton.x) - m_heloLocation.x;
		displacement.y = static_cast<float>(t_event.mouseButton.y) - m_heloLocation.y;
		m_target.x = static_cast<float>(t_event.mouseButton.x);
		m_target.y = static_cast<float>(t_event.mouseButton.y);
		
		lenght = std::sqrtf(displacement.x * displacement.x + displacement.y * displacement.y);
		displacement = displacement / lenght;
		displacement = displacement * m_speed;

		m_heloVelocity = displacement;
		m_heloSound.setPitch(1.0f);
		if (m_target.x > m_heloLocation.x)
		{
			m_direction = Direction::Right;
			m_heloSprite.setScale(1.0f, 1.0f);

		}
		else
		{
			m_direction = Direction::Left;
			m_heloSprite.setScale(-1.0f, 1.0f);
		}
	}

}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	animateHelicopter();
	moveHelo();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	m_window.draw(m_heloSprite);
	
	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("SFML Game");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_heloTexture.loadFromFile("ASSETS\\IMAGES\\helicopter.PNG"))
	{
		std::cout << "helo sprite problem" << std::endl;
	}
	m_heloSprite.setTexture(m_heloTexture);
	m_heloSprite.setPosition(m_heloLocation);
	m_heloSprite.setTextureRect(sf::IntRect{ 0,0,180, 64 });
	m_heloSprite.setOrigin(90.0f, 0.0f);

	if (!m_logoTexture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}
	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setPosition(300.0f, 180.0f);
}

void Game::setupAudio()
{
	if (!m_heloSoundBuffer.loadFromFile("ASSETS\\AUDIO\\helicopter.wav"))
	{
		std::cout <<  "sound issue" << std::endl;
	}
	m_heloSound.setBuffer(m_heloSoundBuffer);
	m_heloSound.setPitch(0.5);
	m_heloSound.play();
	m_heloSound.setLoop(true);

	if (!m_music.openFromFile("ASSETS\\AUDIO\\music.ogg"))
	{
		std::cout << "problem with music" << std::endl;
	}
	m_music.play();
}

void Game::animateHelicopter()
{
	int const FRAME_HEIGHT = 64;
	m_framecounter += m_frameIncrement;
	int frame = static_cast<int>(m_framecounter);
	frame = frame % 4;
	if (frame != m_frame)
	{
		m_frame = frame;
		m_heloSprite.setTextureRect(sf::IntRect{ 0,frame * FRAME_HEIGHT,180,FRAME_HEIGHT });
	}
}

void Game::moveHelo()
{
	if (m_direction != Direction::None)
	{
		m_heloLocation += m_heloVelocity; // equation of motion
	}
	if (m_direction == Direction::Right
		&& m_heloLocation.x > m_target.x)
	{
		m_direction = Direction::None;
		m_frameIncrement = 0.25f;
		m_heloSound.setPitch(0.5);
	}
	if (m_direction == Direction::Left
		&& m_heloLocation.x < m_target.x)
	{
		m_direction = Direction::None;
		m_frameIncrement = 0.25f;
		m_heloSound.setPitch(0.5);
	}

	m_heloSprite.setPosition(m_heloLocation);
}
