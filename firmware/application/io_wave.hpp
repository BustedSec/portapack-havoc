/*
 * Copyright (C) 2016 Jared Boone, ShareBrained Technology, Inc.
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

#pragma once

#include "io_file.hpp"

#include "file.hpp"
#include "optional.hpp"

#include <cstddef>
#include <cstdint>

struct fmt_pcm_t {
	constexpr fmt_pcm_t(
		const uint32_t sampling_rate
	) : nSamplesPerSec { sampling_rate },
		nAvgBytesPerSec { nSamplesPerSec * nBlockAlign }
	{
	}

private:
	uint8_t ckID[4] { 'f', 'm', 't', ' ' };
	uint32_t cksize { 16 };
	uint16_t wFormatTag { 0x0001 };
	uint16_t nChannels { 1 };
	uint32_t nSamplesPerSec;
	uint32_t nAvgBytesPerSec;
	uint16_t nBlockAlign { 2 };
	uint16_t wBitsPerSample { 16 };
};

struct data_t {
	constexpr data_t(
		const uint32_t size
	) : cksize { size }
	{
	}

private:
	uint8_t ckID[4] { 'd', 'a', 't', 'a' };
	uint32_t cksize { 0 };
};

struct header_t {
	constexpr header_t(
		const uint32_t sampling_rate,
		const uint32_t data_chunk_size
	) : cksize { sizeof(header_t) + data_chunk_size - 8 },
		fmt { sampling_rate },
		data { data_chunk_size }
	{
	}

private:
	uint8_t riff_id[4] { 'R', 'I', 'F', 'F' };
	uint32_t cksize { 0 };
	uint8_t wave_id[4] { 'W', 'A', 'V', 'E' };
	fmt_pcm_t fmt;
	data_t data;
};

class WAVFileReader : public FileReader {
public:
	WAVFileReader() = default;
	
	WAVFileReader(const WAVFileReader&) = delete;
	WAVFileReader& operator=(const WAVFileReader&) = delete;
	WAVFileReader(WAVFileReader&&) = delete;
	WAVFileReader& operator=(WAVFileReader&&) = delete;
	
	virtual ~WAVFileReader() = default;

	bool open(const std::filesystem::path& path);
	void rewind();
	uint32_t ms_duration();
	//int seek_mss(const uint16_t minutes, const uint8_t seconds, const uint32_t samples);
	uint16_t channels();
	uint32_t sample_rate();
	uint32_t data_size();
	uint16_t bits_per_sample();
	std::string title();
	
private:
	struct fmt_pcm_t {
		uint8_t ckID[4];		// fmt 
		uint32_t cksize;
		uint16_t wFormatTag;
		uint16_t nChannels;
		uint32_t nSamplesPerSec;
		uint32_t nAvgBytesPerSec;
		uint16_t nBlockAlign;
		uint16_t wBitsPerSample;
	};

	struct data_t {
		uint8_t ckID[4];		// data
		uint32_t cksize;
	};

	struct header_t {
		uint8_t riff_id[4];		// RIFF
		uint32_t cksize;
		uint8_t wave_id[4];		// WAVE
		fmt_pcm_t fmt;
		data_t data;
	};

	header_t header { };

	uint32_t data_start { };
	uint32_t bytes_per_sample { };
	uint32_t data_size_ { 0 };
	uint32_t sample_rate_ { };
	std::string title_string { };
	std::filesystem::path last_path { };
};

class WAVFileWriter : public FileWriter {
public:
	WAVFileWriter() = default;

	WAVFileWriter(const WAVFileWriter&) = delete;
	WAVFileWriter& operator=(const WAVFileWriter&) = delete;
	WAVFileWriter(WAVFileWriter&&) = delete;
	WAVFileWriter& operator=(WAVFileWriter&&) = delete;

	~WAVFileWriter() {
		update_header();
	}

	Optional<File::Error> create(
		const std::filesystem::path& filename,
		size_t sampling_rate
	);

private:
	uint32_t sampling_rate { 0 };

	Optional<File::Error> update_header();
};
