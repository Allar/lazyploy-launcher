// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#pragma once

/**
* Implements a widget for form input field labels with optional validation errors.
*/
class SCookAndDeploy
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SCookAndDeploy) { }
	SLATE_END_ARGS()

public:

	/**
	* Constructs the widget.
	*
	* @param InArgs The Slate argument list.
	*/
	void Construct(const FArguments& InArgs, TSharedRef<FLazyployLauncherClient> InClient, const TSharedRef<ISlateStyle>& InStyle);

	void LoadOptionsFromConfig();
	void SaveOptionsToConfig();

	bool IsCookingEnabled() const;
	bool IsBuildManagerEnabled() const;
	bool IsBuildManagerSettingsEnabled() const;
	FReply StartCook();

private:

	TSharedPtr<FLazyployLauncherClient> Client;

	// Widgets
	TSharedPtr<class SCookProgress> CookProgress;

	// Standard Cook Options
	TSharedPtr<SCheckboxOption> WindowsCheckboxOption;
	TSharedPtr<SCheckboxOption> WindowsServerCheckboxOption;
	TSharedPtr<SCheckboxOption> LinuxCheckboxOption;
	TSharedPtr<SCheckboxOption> LinuxServerCheckboxOption;
	TSharedPtr<SCheckboxOption> PakCheckboxOption;
	TSharedPtr<SCheckboxOption> CompressCheckboxOption;
	TSharedPtr<SCheckboxOption> IterateCheckboxOption;
	TSharedPtr<SCheckboxOption> StripVersionCheckboxOption;

	// Post Cook Options
	TSharedPtr<SCheckboxOption> WinServerSteamFixCheckboxOption;
	TSharedPtr<SCheckboxOption> LinuxServerSteamFixCheckboxOption;

	// Post Stage Options
	TSharedPtr<SCheckboxOption> StripDebugFilesCheckboxOption;
	TSharedPtr<SCheckboxOption> ZipBuildCheckboxOption;
	TSharedPtr<SCheckboxOption> DeployToBuildManagerCheckboxOption;

	// Build Manager Options
	TSharedPtr<SEditableTextBox> BuildManagerUrlTextBox;
	TSharedPtr<SEditableTextBox> BuildDescriptionTextBox;
};
