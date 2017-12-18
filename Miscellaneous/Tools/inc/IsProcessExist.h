/*
判断进程是否存在
*/

#pragma once

#include <string>
using std::string;


class CIsProcessExist
{
public:
    CIsProcessExist(const string& ProcessFileName);

    bool m_bExist;
};

