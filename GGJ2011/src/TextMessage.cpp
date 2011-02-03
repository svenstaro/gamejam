#include "TextMessage.hpp"

#include "GameApp.hpp"

TextMessage::TextMessage(const std::string& text) {
	mText = text;
	mLifeTime = 0;
}

void TextMessage::SetText(const std::string& text) {
	mText = text;
}

const std::string& TextMessage::GetText() const {
	return mText;
}

void TextMessage::Update(float time_delta) {
	mLifeTime += time_delta;
}

void TextMessage::Draw(sf::RenderTarget* target) {
	float total = 3;
	if(mLifeTime < total) {
		sf::Text t(mText);
		t.SetFont(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetFont("custom"));
		t.SetCharacterSize(30);
		t.SetPosition(WIDTH / 2 - t.GetRect().Width / 2,
					  HEIGHT - t.GetRect().Height - 10 - 20 / total * mLifeTime);
		t.SetColor(sf::Color(255,255,255,255 - 255 / total * mLifeTime));
		target->Draw(t);
	}
}
