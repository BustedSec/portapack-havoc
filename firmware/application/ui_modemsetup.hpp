/*
 * Copyright (C) 2014 Jared Boone, ShareBrained Technology, Inc.
 * Copyright (C) 2016 Furrtek
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "modems.hpp"
#include "serializer.hpp"

#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"

namespace ui {

class ModemSetupView : public View {
public:
	ModemSetupView(NavigationView& nav);
	
	void focus() override;
	
	std::string title() const override { return "Modem setup"; };

private:
	void update_freq(rf::Frequency f);
	
	Labels labels {
		{ { 2 * 8, 4 * 8 }, "Frequency:", Color::light_grey() },
		{ { 2 * 8, 11 * 8 }, "Speed:     Bps", Color::light_grey() },
		{ { 2 * 8, 13 * 8 }, "Mark:      Hz", Color::light_grey() },
		{ { 2 * 8, 15 * 8 }, "Space:     Hz", Color::light_grey() },
		{ { 140, 13 * 8 }, "BW:   kHz", Color::light_grey() },
		{ { 140, 15 * 8 }, "Repeat:", Color::light_grey() },
		{ { 2 * 8, 19 * 8 }, "Modem preset:", Color::light_grey() },
		{ { 2 * 8, 22 * 8 }, "Serial format:", Color::light_grey() }
	};
	
	Button button_setfreq {
		{ 13 * 8, 3 * 8, 12 * 8, 32 },
		"----.----"
	};

	NumberField field_baudrate {
		{ 64, 88 },
		5,
		{ 50, 9600 },
		25,
		' '
	};

	NumberField field_mark {
		{ 64, 104 },
		5,
		{ 100, 15000 },
		25,
		' '
	};
	
	NumberField field_space {
		{ 64, 120 },
		5,
		{ 100, 15000 },
		25,
		' '
	};
	
	NumberField field_bw {
		{ 172, 104 },
		2,
		{ 1, 50 },
		1,
		' '
	};
	
	NumberField field_repeat {
		{ 204, 120 },
		2,
		{ 1, 99 },
		1,
		' '
	};
	
	OptionsField options_modem {
		{ 16 * 8, 19 * 8 },
		7,
		{
		}
	};
	
	SymField sym_format {
		{ 16 * 8, 22 * 8 },
		4,
		SymField::SYMFIELD_DEF
	};
	
	Button button_set_modem {
		{ 24 * 8, 19 * 8 - 4, 5 * 8, 24 },
		"SET"
	};
	
	Button button_save {
		{ 72, 250, 96, 40 },
		"Save"
	};
};

} /* namespace ui */
