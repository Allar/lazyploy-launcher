// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "../AllarBuilderClientApp.h"
#include "SAllarBuilderFormLabel.h"
#include "SProjectPicker.h"


#define LOCTEXT_NAMESPACE "SProjectPicker"


/* SProjectPicker structors
*****************************************************************************/

SProjectPicker::~SProjectPicker()
{
	if (Model.IsValid())
	{
		//Model->OnProfileSelected().RemoveAll(this);
	}
}


/* SProjectPicker interface
*****************************************************************************/

void SProjectPicker::Construct(const FArguments& InArgs, const FAllarBuilderClientRef& InModel)
{
	Model = InModel;

	ChildSlot
	[
		MakeProjectWidget()
	];
}


/* SProjectPicker implementation
*****************************************************************************/

TSharedRef<SWidget> SProjectPicker::MakeProjectMenuWidget()
{
	FMenuBuilder MenuBuilder(true, NULL);
	{
		const TArray<FString>& AvailableGames = FGameProjectHelper::GetAvailableGames();
		for (int32 Index = 0; Index < AvailableGames.Num(); ++Index)
		{
			FString ProjectPath = FPaths::RootDir() / AvailableGames[Index] / AvailableGames[Index] + TEXT(".uproject");
			FUIAction ProjectAction(FExecuteAction::CreateSP(this, &SProjectPicker::HandleProjectMenuEntryClicked, ProjectPath));
			MenuBuilder.AddMenuEntry(FText::FromString(AvailableGames[Index]), FText::FromString(ProjectPath), FSlateIcon(), ProjectAction);
		}

		MenuBuilder.AddMenuSeparator();

		FUIAction BrowseAction(FExecuteAction::CreateSP(this, &SProjectPicker::HandleProjectMenuEntryClicked, FString()));
		MenuBuilder.AddMenuEntry(LOCTEXT("BrowseAction", "Browse..."), LOCTEXT("BrowseActionHint", "Browse for a project on your computer"), FSlateIcon(), BrowseAction);
	}

	return MenuBuilder.MakeWidget();
}

TSharedRef<SWidget> SProjectPicker::MakeProjectWidget()
{
	TSharedRef<SWidget> Widget = SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		.Padding(4.0)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(0.0f, 0.0f, 16.0f, 0.0f)
					[
						SNew(SAllarBuilderFormLabel)
							.ErrorToolTipText(NSLOCTEXT("AllarBuilderBuildValidation", "NoProjectSelectedError", "A Project must be selected."))
							.ErrorVisibility(this, &SProjectPicker::HandleValidationErrorIconVisibility)
							.LabelText(LOCTEXT("ProjectComboBoxLabel", "Project:"))
					]

				+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						// project selector
						SNew(SComboButton)
							.ButtonContent()
							[
								SNew(STextBlock)
									.Text(this, &SProjectPicker::HandleProjectComboButtonText)
									.TextStyle(FAllarBuilderClientStyle::Get(), TEXT("ProjectPicker.Text"))
							]
							.ContentPadding(FMargin(4.0f, 2.0f))
							.MenuContent()
							[
								MakeProjectMenuWidget()
							]
						.ToolTipText(this, &SProjectPicker::HandleProjectComboButtonToolTip)
					]
		];

	return Widget;
}


/* SProjectPicker callbacks
*****************************************************************************/

FText SProjectPicker::HandleProjectComboButtonText() const
{
	FString Name = Model->GetProjectName();
	if (!Name.IsEmpty())
	{
		return FText::FromString(Name);
	}

	return LOCTEXT("SelectProjectText", "Select...");
}


FText SProjectPicker::HandleProjectComboButtonToolTip() const
{
	return LOCTEXT("SelectProjectText", "Select or browse for a project");
}


void SProjectPicker::HandleProjectMenuEntryClicked(FString ProjectPath)
{
	if (ProjectPath.IsEmpty())
	{
		FString DefaultPath = FPaths::RootDir();

		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

		TArray<FString> OutFiles;
		if (FDesktopPlatformModule::Get()->OpenFileDialog(ParentWindowHandle, LOCTEXT("SelectProjectDialogTitle", "Select a project").ToString(), DefaultPath, TEXT(""), TEXT("Project files (*.uproject)|*.uproject"), EFileDialogFlags::None, OutFiles))
		{
			SetProjectPath(OutFiles[0]);
		}
	}
	else
	{
		SetProjectPath(ProjectPath);
	}
}


void SProjectPicker::SetProjectPath(FString ProjectPath)
{
	Model->SetProjectPath(ProjectPath);
}


EVisibility SProjectPicker::HandleValidationErrorIconVisibility() const
{
	if (Model->GetProjectPath().IsEmpty())
	{
		return EVisibility::Visible;
	}
	return EVisibility::Collapsed;
}


#undef LOCTEXT_NAMESPACE
