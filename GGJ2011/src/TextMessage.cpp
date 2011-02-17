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
	float total = 5;
	int margin = 10;
	if(mLifeTime < total) {
		sf::Text t(mText);
		t.SetFont(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetFont("custom"));
		t.SetCharacterSize(20);
		while(t.GetRect().Width > target->GetWidth() - 2 * margin)
			t.SetCharacterSize(t.GetCharacterSize() - 1);

		t.SetPosition(target->GetWidth() / 2 - t.GetRect().Width / 2,
					  target->GetHeight() - t.GetRect().Height - margin - 70 / total * mLifeTime);
		t.SetColor(sf::Color(255,255,255,255 * sin(mLifeTime / total * PI)));
		target->Draw(t);
	}
}
