#pragma once
LONG WINAPI TopExceptionCallback(struct _EXCEPTION_POINTERS *pei);
class CTrackThread : public wxThread
{
public:
    CTrackThread();
    virtual void *Entry();
    virtual void OnExit();
private:

};
class CBeepThread : public wxThread
{
public:
	CBeepThread();
	virtual void *Entry();
	virtual void OnExit();
private:

};