/**
 * @file CSerialPortHotSwap.h
 * @author itas109 (itas109@gmail.com) \n\n
 * Blog : http://blog.csdn.net/itas109 \n
 * Github : https://github.com/itas109 \n
 * Gitee : https://gitee.com/itas109 \n
 * QQ Group : 12951803
 * @brief CSerialPort extend class,used to monitor port insert and remove CSerialPort的扩展类,用于监测串口的插入与移除
 * @version 1.0.0.200510
 * @date 2020-05-10
 * 
 * @copyright 
 * Copyright(c)2020 itas109
 * CSerialPortHotSwap is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at :
 * http ://license.coscl.org.cn/MulanPSL2 
 *	THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON - INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *	See the Mulan PSL v2 for more details.
 */

#pragma once

#include <windows.h>
#include <string>

#include "sigslot.h"
using namespace sigslot;

namespace itas109
{
class CSerialPortHotSwap:public has_slots<>
{
	public:
		CSerialPortHotSwap();
		~CSerialPortHotSwap();

		std::string getVersion();

	public:
		void onPortChange(int portStatus, std::string portName);
		static sigslot::signal2<int, std::string> portChangeSignal;//sigslot

	private:
		static LRESULT CALLBACK  _WinProcCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static unsigned int __stdcall ThrdFunc(LPVOID lpParam);
	private:
		BOOL initMessageOnlyWindows();
		void MessagePump();

	private:
		HWND hWnd;
	};
}

