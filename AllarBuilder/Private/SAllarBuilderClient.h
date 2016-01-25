// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AllarBuilderClient.h"

#if !CRASH_REPORT_UNATTENDED_ONLY

/**
* UI for the crash report client app
*/
class SAllarBuilderClient : public SCompoundWidget
{
public:
	/**
	* Slate arguments
	*/
	SLATE_BEGIN_ARGS(SAllarBuilderClient)
	{
	}

	SLATE_END_ARGS()

	/**
	* Construct this Slate ui
	* @param InArgs Slate arguments, not used
	* @param Client Allar Builder client implementation object
	*/
	void Construct(const FArguments& InArgs, TSharedRef<FAllarBuilderClient> Client);

private:

	/** Crash report client implementation object */
	TSharedPtr<FAllarBuilderClient> AllarBuilderClient;
};

#endif // !CRASH_REPORT_UNATTENDED_ONLY
