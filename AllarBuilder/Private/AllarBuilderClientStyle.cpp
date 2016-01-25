// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "AllarBuilderClientApp.h"
#include "AllarBuilderClientStyle.h"

TSharedPtr< FSlateStyleSet > FAllarBuilderClientStyle::StyleSet = nullptr;

void FAllarBuilderClientStyle::Initialize()
{
	if (!StyleSet.IsValid())
	{
		StyleSet = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	}
}

void FAllarBuilderClientStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
	ensure(StyleSet.IsUnique());
	StyleSet.Reset();
}

#define TTF_FONT(RelativePath, ...) FSlateFontInfo(ContentFromEngine(TEXT(RelativePath), TEXT(".ttf")), __VA_ARGS__)
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush(ContentFromEngine(TEXT(RelativePath), TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush(ContentFromEngine(TEXT(RelativePath), TEXT(".png")), __VA_ARGS__)
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush(ContentFromEngine(RelativePath, TEXT(".png")), __VA_ARGS__)

namespace
{
	FString ContentFromEngine(const FString& RelativePath, const TCHAR* Extension)
	{
		static const FString ContentDir = FPaths::EngineDir() / TEXT("Content/Slate");
		return ContentDir / RelativePath + Extension;
	}
}

TSharedRef< FSlateStyleSet > FAllarBuilderClientStyle::Create()
{
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon48x48(48.0f, 48.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);

	TSharedRef<FSlateStyleSet> StyleRef = MakeShareable(new FSlateStyleSet("FAllarBuilderClientStyle"));
	FSlateStyleSet& Style = StyleRef.Get();

	const FTextBlockStyle DefaultText = FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/Roboto-Bold", 10))
		.SetColorAndOpacity(FSlateColor::UseForeground())
		.SetShadowOffset(FVector2D::ZeroVector)
		.SetShadowColorAndOpacity(FLinearColor::Black);

	// Set the client app styles
	Style.Set(TEXT("ProjectPicker.Text"), FTextBlockStyle(DefaultText)
		.SetFontSize(24)
		);
	
	Style.Set(TEXT("ProjectPicker.ErrorImage"), new IMAGE_BRUSH("Icons/icon_error_16x", Icon48x48));


	Style.Set(TEXT("Code"), FTextBlockStyle(DefaultText)
		.SetFont(TTF_FONT("Fonts/Roboto-Regular", 8))
		.SetColorAndOpacity(FSlateColor(FLinearColor::White * 0.8f))
		);

	Style.Set(TEXT("Title"), FTextBlockStyle(DefaultText)
		.SetFont(TTF_FONT("Fonts/Roboto-Bold", 12))
		);

	Style.Set(TEXT("Status"), FTextBlockStyle(DefaultText)
		.SetColorAndOpacity(FSlateColor::UseSubduedForeground())
		);

	FSlateBrush* GenericWhiteBox = new IMAGE_BRUSH("Old/White", Icon16x16);

	// Scrollbar
	const FScrollBarStyle ScrollBar = FScrollBarStyle()
		.SetVerticalTopSlotImage(IMAGE_BRUSH("Common/Scrollbar_Background_Vertical", FVector2D(8, 8)))
		.SetVerticalBottomSlotImage(IMAGE_BRUSH("Common/Scrollbar_Background_Vertical", FVector2D(8, 8)))
		.SetHorizontalTopSlotImage(IMAGE_BRUSH("Common/Scrollbar_Background_Horizontal", FVector2D(8, 8)))
		.SetHorizontalBottomSlotImage(IMAGE_BRUSH("Common/Scrollbar_Background_Horizontal", FVector2D(8, 8)))
		.SetNormalThumbImage(BOX_BRUSH("Common/Scrollbar_Thumb", FMargin(4.f / 16.f)))
		.SetDraggedThumbImage(BOX_BRUSH("Common/Scrollbar_Thumb", FMargin(4.f / 16.f)))
		.SetHoveredThumbImage(BOX_BRUSH("Common/Scrollbar_Thumb", FMargin(4.f / 16.f)));

	// SEditableTextBox defaults...
	const FEditableTextBoxStyle NormalEditableTextBoxStyle = FEditableTextBoxStyle()
		.SetBackgroundImageNormal(*GenericWhiteBox)
		.SetBackgroundImageHovered(*GenericWhiteBox)
		.SetBackgroundImageFocused(*GenericWhiteBox)
		.SetBackgroundImageReadOnly(*GenericWhiteBox)
		.SetScrollBarStyle(ScrollBar);
	{
		Style.Set("NormalEditableTextBox", NormalEditableTextBoxStyle);
	}

	// RichText
	const FTextBlockStyle AllarBuilderDataStyle = FTextBlockStyle()
		.SetFont(TTF_FONT("Testing/Fonts/Roboto-Italic", 9))
		.SetColorAndOpacity(FSlateColor(FLinearColor::White * 0.5f))
		.SetShadowOffset(FVector2D::ZeroVector)
		.SetShadowColorAndOpacity(FLinearColor::Black);

	Style.Set("AllarBuilderDataStyle", AllarBuilderDataStyle);

	FButtonStyle DarkHyperlinkButton = FButtonStyle()
		.SetNormal(BORDER_BRUSH("Old/HyperlinkDotted", FMargin(0, 0, 0, 3 / 16.0f), FSlateColor(FLinearColor::White * 0.5f)))
		.SetPressed(FSlateNoResource())
		.SetHovered(BORDER_BRUSH("Old/HyperlinkUnderline", FMargin(0, 0, 0, 3 / 16.0f), FSlateColor(FLinearColor::White * 0.5f)));

	const FHyperlinkStyle DarkHyperlink = FHyperlinkStyle()
		.SetUnderlineStyle(DarkHyperlinkButton)
		.SetTextStyle(AllarBuilderDataStyle)
		.SetPadding(FMargin(0.0f));

	Style.Set("RichText.Hyperlink", DarkHyperlink);

	return StyleRef;
}

#undef TTF_FONT

const ISlateStyle& FAllarBuilderClientStyle::Get()
{
	return *StyleSet;
}
