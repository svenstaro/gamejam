#ifndef TEXTMESSAGE_HPP
#define TEXTMESSAGE_HPP

#include <SFML/Graphics.hpp>

class TextMessage {
public:
	TextMessage(const std::string& text);

	void SetText(const std::string& text);
	const std::string& GetText() const;

	void Update(float time_delta);
	void Draw(sf::RenderTarget* target);
private:
	std::string mText;
	float mLifeTime;
};

#endif // TEXTMESSAGE_HPP
