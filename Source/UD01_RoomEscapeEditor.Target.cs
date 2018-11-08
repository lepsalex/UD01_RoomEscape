// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UD01_RoomEscapeEditorTarget : TargetRules
{
	public UD01_RoomEscapeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "UD01_RoomEscape" } );
	}
}
