#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

std::condition_variable     switchState;
std::condition_variable     stopPlay;

class MusicPlayer
{
public:
	MusicPlayer() 
	{
		_currState = MusicPause;
		_currSound = 0;
		_isLoop    = false;
	}

	~MusicPlayer()
	{
		for (auto it = _music.begin(); it != _music.end(); ++it)
			delete (*it);
	}

	void SetMusic(const std::vector<std::string> musicFiles)
	{
		for (auto it = musicFiles.begin(); it != musicFiles.end(); ++it)
		{
			sf::Music* newMusic = new sf::Music;
			newMusic->openFromFile((*it));
			_music.push_back(newMusic);
		};
	}

	void Play ()
	{
		_music[_currSound]->play();
		_currState = MusicPlay;
		_isLoop = true;

		std::mutex mutex;
		std::unique_lock<std::mutex> lock(mutex);

		while (_isLoop)
		{
			if (switchState.wait_for(lock, std::chrono::seconds(1)) == std::cv_status::timeout)
			{
				if (_music[_currSound]->getStatus() == sf::Music::Stopped)
				{
					_currSound = (++_currSound) % _music.size();
					_music[_currSound]->play();
				}
			}
			else
			{
				if (_currState == MusicPlay)
				{
					_music[_currSound]->pause();
					_currState = MusicPause;
				}
				else
				{
					_music[_currSound]->play();
					_currState = MusicPlay;
				}
			}
		}
	}

	void ChangeState()
	{
		switchState.notify_one();
	}

	void StopPlaying()
	{
		_isLoop = false;
	}

private:
	enum
	{
		MusicPlay  = 1,
		MusicPause = 2
	} _currState;

	bool _isLoop;
	int  _currSound;
	std::vector<sf::Music*> _music;
};


int main()
{
	std::vector<std::string> files ({"1.wav", "2.wav", "3.wav"});
	sf::Event event;

	sf::RenderWindow window(sf::VideoMode(800, 600), "MyWindow");

	MusicPlayer player;

	player.SetMusic(files);

	auto playerCall = [&player]() { player.Play(); };
	std::thread playerThread(playerCall);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				player.StopPlaying();
				window.close();
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			player.StopPlaying();
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space )) player.ChangeState ();

		window.clear(sf::Color::Red);
		window.display();
	}

	playerThread.join();

	return 0;
}