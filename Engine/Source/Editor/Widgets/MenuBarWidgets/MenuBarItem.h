//
// Created by MarvelLi on 2024/2/29.
//

#pragma once
#include "UIWidget.h"

class MenuBarItem : public UIWidget
{
public:
	explicit MenuBarItem(const String& Name)
		: UIWidget(Name) {}

	virtual void OnClick() = 0;

	virtual void Draw() override {}
};

template <class T>
concept IsMenuBarItem = std::is_base_of_v<MenuBarItem, T>;
