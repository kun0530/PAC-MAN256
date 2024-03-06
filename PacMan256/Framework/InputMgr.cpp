#include "pch.h"
#include "InputMgr.h"

std::map<Axis, AxisInfo> InputMgr::axisInfoMap;
std::list<sf::Keyboard::Key> InputMgr::downList;
std::list<sf::Keyboard::Key> InputMgr::upList;
std::list<sf::Keyboard::Key> InputMgr::ingList;
sf::Vector2f InputMgr::mousePos;

void InputMgr::Init()
{
    // Horizontal
    AxisInfo infoH;
    infoH.axis = Axis::Horizontal;
    infoH.positives.push_back(sf::Keyboard::D);
    infoH.positives.push_back(sf::Keyboard::Right);

    infoH.negatives.push_back(sf::Keyboard::A);
    infoH.negatives.push_back(sf::Keyboard::Left);

    infoH.sensi = 8.f;
    infoH.value = 0.f;
    axisInfoMap.insert({ infoH.axis, infoH });

    // Vertical
    AxisInfo infoV;
    infoV.axis = Axis::Vertical;
    infoV.positives.push_back(sf::Keyboard::S);
    infoV.positives.push_back(sf::Keyboard::Down);
    infoV.negatives.push_back(sf::Keyboard::W);
    infoV.negatives.push_back(sf::Keyboard::Up);
    infoV.sensi = 8.f;
    infoV.value = 0.f;
    axisInfoMap.insert({ infoV.axis, infoV });
}

void InputMgr::UpdateEvent(const sf::Event& ev)
{
    switch (ev.type)
    {
    case sf::Event::KeyPressed:
        if (!GetKey(ev.key.code))
        {
            ingList.push_back(ev.key.code);
            downList.push_back(ev.key.code);
        }
        break;
    case sf::Event::KeyReleased:
        ingList.remove(ev.key.code);
        upList.push_back(ev.key.code);
        break;
    case sf::Event::MouseButtonPressed:
        if (!GetMouseButton(ev.mouseButton.button))
        {
            sf::Keyboard::Key button = MouseButtonToKey(ev.mouseButton.button);
            ingList.push_back(button);
            downList.push_back(button);
        }
        break;
    case sf::Event::MouseButtonReleased:
        {
            sf::Keyboard::Key button = MouseButtonToKey(ev.mouseButton.button);
            ingList.remove(button);
            upList.push_back(button);
        }
        break;
    }
}

void InputMgr::Update(float dt)
{
    mousePos = (sf::Vector2f)sf::Mouse::getPosition(FRAMEWORK.GetWindow());

    for (auto& pair : axisInfoMap)
    {
        AxisInfo& axisInfo = pair.second;
        float raw = GetAxisRaw(axisInfo.axis);  // -1 0 1
        float speed = raw;
        if (speed == 0.f && axisInfo.value != 0.f)
        {
            speed = axisInfo.value > 0.f ? -1.f : 1.f;
        }
        else
        {
            speed *= 2.f;
        }
 
        axisInfo.value += speed * axisInfo.sensi * dt;
        axisInfo.value = Utils::Clamp(axisInfo.value, -1.f, 1.f);

        bool over = (speed > 0) ? (axisInfo.value > 0.f) : (axisInfo.value < 0.f);
        // bool approximately = abs(axisInfo.value) < speed * axisInfo.sensi * dt;
        if (raw == 0.f && over)
        {
            axisInfo.value = 0.f;
        }
    }
}

void InputMgr::Clear()
{
    downList.clear();
    upList.clear();
}

float InputMgr::GetAxisRaw(Axis axis)
{
    auto findInfo = axisInfoMap.find(axis);
    if (findInfo == axisInfoMap.end())
        return 0.f;

    const AxisInfo& info = findInfo->second;
    auto it = ingList.rbegin();
    while (it != ingList.rend())
    {
        sf::Keyboard::Key key = *it;
        if (std::find(info.positives.begin(), info.positives.end(), key) != info.positives.end())
        {
            return 1.f;
        }
        if (std::find(info.negatives.begin(), info.negatives.end(), key) != info.negatives.end())
        {
            return -1.f;
        }
        ++it;
    }
    return 0.0f;
}

float InputMgr::GetAxis(Axis axis)
{
    auto findInfo = axisInfoMap.find(axis);
    if (findInfo == axisInfoMap.end())
        return 0.f;

    return findInfo->second.value;
}

sf::Vector2i InputMgr::GetAxisOne()
{
    const AxisInfo& infoH = axisInfoMap.find(Axis::Horizontal)->second;
    const AxisInfo& infoV = axisInfoMap.find(Axis::Vertical)->second;

    auto it = downList.rbegin();
    while (it != downList.rend())
    {
        sf::Keyboard::Key key = *it;

        if (std::find(infoH.positives.begin(), infoH.positives.end(), key) != infoH.positives.end())
        {
            return sf::Vector2i(1, 0);
        }
        if (std::find(infoH.negatives.begin(), infoH.negatives.end(), key) != infoH.negatives.end())
        {
            return sf::Vector2i(-1, 0);
        }
        if (std::find(infoV.positives.begin(), infoV.positives.end(), key) != infoV.positives.end())
        {
            return sf::Vector2i(0, 1);
        }
        if (std::find(infoV.negatives.begin(), infoV.negatives.end(), key) != infoV.negatives.end())
        {
            return sf::Vector2i(0, -1);
        }
        ++it;
    }
    return sf::Vector2i(0, 0);
}

bool InputMgr::GetKeyDown(sf::Keyboard::Key key)
{
    //std::list<sf::Keyboard::Key>::iterator it = downList.begin

    //for (auto it = downList.begin(); it != downList.end(); ++it)
    //{
    //    if (*it == key)
    //        return true;
    //}

    //for (auto k : downList)
    //{
    //    if (k == key)
    //        return true;
    //}

    return std::find(downList.begin(), downList.end(), key) != downList.end();
}

bool InputMgr::GetKeyUp(sf::Keyboard::Key key)
{
    return std::find(upList.begin(), upList.end(), key) != upList.end();
}

bool InputMgr::GetKey(sf::Keyboard::Key key)
{
    return std::find(ingList.begin(), ingList.end(), key) != ingList.end();
}

bool InputMgr::GetMouseButtonDown(sf::Mouse::Button button)
{
    return std::find(downList.begin(), downList.end(), MouseButtonToKey(button)) != downList.end();
}

bool InputMgr::GetMouseButtonUp(sf::Mouse::Button button)
{
    return std::find(upList.begin(), upList.end(), MouseButtonToKey(button)) != upList.end();;
}

bool InputMgr::GetMouseButton(sf::Mouse::Button button)
{
    return std::find(ingList.begin(), ingList.end(), MouseButtonToKey(button)) != ingList.end();;
}
