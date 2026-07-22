#include "screens/MenuScreen.h"

#include "screens/ArmoryScreen.h"
#include "screens/ShopScreen.h"
#include "Globals.h"
#include "SpriteUtils.h"

namespace {
    constexpr float kButtonHeight = 56.f;
    constexpr float kButtonSpacing = 18.f;
    constexpr unsigned kTitleSize = 56;
}

MenuScreen::MenuScreen(ScreenNavigator& navigator)
    : navigator_(navigator)
{
    buttonLabels_ = {
        "Local", "Multiplayer",
        "Player", "Armory", "Settings", "Exit"
    };
}

void MenuScreen::initializeAssets()
{
    if (assetsReady_) {
        return;
    }
    initializeFont();
    backgroundSprite_ = loadBackgroundSprite(&backgroundTexture_, "menu_v2");
    assetsReady_ = true;
}

void MenuScreen::onEnter(RenderContext& context)
{
    (void)context;
    initializeAssets();
}

void MenuScreen::onExit(RenderContext& context)
{
    (void)context;
}

void MenuScreen::onResize(RenderContext& context)
{
    rebuildLayout(context.logicalBounds());
}

void MenuScreen::rebuildLayout(const sf::FloatRect& root)
{
    backgroundLayer_ = Layout::fromNormalized(root, 0.f, 0.f, 1.f, 1.f);
    // Darkened left panel in menu_v2 (~left 38%).
    uiLayer_ = Layout::fromNormalized(root, 0.04f, 0.08f, 0.34f, 0.84f);
    titlePanel_ = Layout::uiBar(uiLayer_.toFloatRect(), true, 100.f);
    menuColumn_ = Layout::inset(uiLayer_.toFloatRect(), 0.f, 110.f, 0.f, 40.f);

    Layout::scaleSpriteToCover(backgroundSprite_, backgroundLayer_.toFloatRect(),
        Anchor::CenterLeft);

    const LayoutRect titleSlot = Layout::resolve(Anchor::TopLeft,
        titlePanel_.toFloatRect(), { titlePanel_.width, 80.f });
    titlePosition_ = sf::Vector2f(titleSlot.x, titleSlot.y + 8.f);

    buttons_.clear();
    const auto slots = Layout::verticalStack(menuColumn_.toFloatRect(),
        buttonLabels_.size(), kButtonHeight, kButtonSpacing, Anchor::CenterLeft);
    for (size_t i = 0; i < buttonLabels_.size() && i < slots.size(); ++i) {
        const LayoutRect& slot = slots[i];
        buttons_.push_back(std::make_unique<Button>(
            sf::Vector2f(slot.x, slot.y),
            sf::Vector2f(slot.width, slot.height),
            buttonLabels_[i]));
    }
}

void MenuScreen::handleEvent(const sf::Event& event, RenderContext& context)
{
    if (event.type == sf::Event::MouseButtonPressed
        && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f mouse = context.mapPixelToLogical(
            { event.mouseButton.x, event.mouseButton.y });
        for (size_t i = 0; i < buttons_.size(); ++i) {
            if (buttons_[i]->isClicked(mouse)) {
                selectedIndex_ = static_cast<int>(i);
                return;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased
        && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f mouse = context.mapPixelToLogical(
            { event.mouseButton.x, event.mouseButton.y });
        for (size_t i = 0; i < buttons_.size(); ++i) {
            if (buttons_[i]->isClicked(mouse)) {
                selectedIndex_ = static_cast<int>(i);
                callSelected(getSelectedItem(), context);
                return;
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedIndex_ = (selectedIndex_ + static_cast<int>(buttonLabels_.size()) - 1)
                % static_cast<int>(buttonLabels_.size());
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedIndex_ = (selectedIndex_ + 1) % static_cast<int>(buttonLabels_.size());
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            callSelected(getSelectedItem(), context);
        }
    }
}

void MenuScreen::update(float dt, RenderContext& context)
{
    (void)dt;
    Button::updateAll(context.input.getLogicalMouse(), &context.window());
}

void MenuScreen::draw(sf::RenderTarget& target, RenderContext& context)
{
    (void)context;
    target.draw(backgroundSprite_);

    sf::Text titleText("Skirmish", globalFont2, kTitleSize);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(titlePosition_);
    target.draw(titleText);

    for (size_t i = 0; i < buttons_.size(); ++i) {
        if (static_cast<int>(i) == getSelectedItem()) {
            buttons_[i]->setBackgroundColor(sf::Color(118, 76, 46));
        }
        else {
            buttons_[i]->setBackgroundColor(sf::Color::Transparent);
        }
        buttons_[i]->draw(dynamic_cast<sf::RenderWindow&>(target));
    }
}

int MenuScreen::getSelectedItem() const
{
    return selectedIndex_;
}

void MenuScreen::callSelected(int selected, RenderContext& context)
{
    (void)context;
    switch (selected) {
    case 0:
        navigator_.replaceScreen(std::make_unique<ShopScreen>(navigator_));
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        navigator_.pushScreen(std::make_unique<ArmoryScreen>(navigator_));
        break;
    case 4:
        break;
    case 5:
        navigator_.quitApplication();
        break;
    default:
        break;
    }
}
