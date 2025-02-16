/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "../../drawing/Drawing.h"
#include "../../interface/Viewport.h"
#include "../../paint/Paint.h"
#include "../../paint/Supports.h"
#include "../../paint/tile_element/Paint.TileElement.h"
#include "../../sprites.h"
#include "../../world/Map.h"
#include "../RideData.h"
#include "../TrackData.h"
#include "../TrackPaint.h"

static constexpr const uint32_t MULTI_DIM_STATION_BLOCK_BRAKE_SW_NE_OPEN = 15812;
static constexpr const uint32_t MULTI_DIM_STATION_BLOCK_BRAKE_NW_SE_OPEN = 15813;
static constexpr const uint32_t MULTI_DIM_STATION_BLOCK_BRAKE_SW_NE_CLOSED = 15814;
static constexpr const uint32_t MULTI_DIM_STATION_BLOCK_BRAKE_NW_SE_CLOSED = 15815;

static constexpr const uint32_t MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_SW_NE_OPEN = 16220;
static constexpr const uint32_t MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_NW_SE_OPEN = 16221;
static constexpr const uint32_t MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_SW_NE_CLOSED = 16222;
static constexpr const uint32_t MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_NW_SE_CLOSED = 16223;

static constexpr const uint32_t MULTI_DIM_INVERTED_BLOCK_BRAKE_SW_NE_OPEN = 26551;
static constexpr const uint32_t MULTI_DIM_INVERTED_BLOCK_BRAKE_NW_SE_OPEN = 26552;
static constexpr const uint32_t MULTI_DIM_INVERTED_BLOCK_BRAKE_SW_NE_CLOSED = 26553;
static constexpr const uint32_t MULTI_DIM_INVERTED_BLOCK_BRAKE_NW_SE_CLOSED = 26554;

/** rct2: 0x00792D88 */
static void multi_dimension_rc_track_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15808, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15809, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            if (track_paint_util_should_paint_supports(session.MapPosition))
            {
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15806, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15807, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            if (track_paint_util_should_paint_supports(session.MapPosition))
            {
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
            }
        }
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26227, 0, 0, 32, 20, 1, height + 24, 0, 6,
                    height + 22);
                break;
            case 1:
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26228, 0, 0, 32, 20, 1, height + 24, 0, 6,
                    height + 22);
                break;
        }

        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
        }

        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
}

/** rct2: 0x00792F98, 0x00792FA8, 0x00792FB8 */
static void multi_dimension_rc_track_station(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    static constexpr const uint32_t imageIds[NumOrthogonalDirections][3] = {
        { 15810, MULTI_DIM_STATION_BLOCK_BRAKE_SW_NE_OPEN, MULTI_DIM_STATION_BLOCK_BRAKE_SW_NE_CLOSED },
        { 15811, MULTI_DIM_STATION_BLOCK_BRAKE_NW_SE_OPEN, MULTI_DIM_STATION_BLOCK_BRAKE_NW_SE_CLOSED },
        { 15810, MULTI_DIM_STATION_BLOCK_BRAKE_SW_NE_OPEN, MULTI_DIM_STATION_BLOCK_BRAKE_SW_NE_CLOSED },
        { 15811, MULTI_DIM_STATION_BLOCK_BRAKE_NW_SE_OPEN, MULTI_DIM_STATION_BLOCK_BRAKE_NW_SE_CLOSED },
    };

    if (trackElement.GetTrackType() == TrackElemType::EndStation)
    {
        bool isClosed = trackElement.BlockBrakeClosed();
        uint8_t offset = isClosed ? 2 : 1;
        PaintAddImageAsParentRotated(
            session, direction, imageIds[direction][offset] | session.TrackColours[SCHEME_TRACK], 0, 0, 32, 26, 1, height, 0, 3,
            height + 3);
    }
    else
    {
        PaintAddImageAsParentRotated(
            session, direction, imageIds[direction][0] | session.TrackColours[SCHEME_TRACK], 0, 0, 32, 26, 1, height, 0, 3,
            height + 3);
    }
    track_paint_util_draw_station_metal_supports_2(session, direction, height, session.TrackColours[SCHEME_SUPPORTS], 11);

    const auto* stationObj = ride.GetStationObject();
    bool hasFence;
    if (direction == 0 || direction == 2)
    {
        hasFence = track_paint_util_has_fence(EDGE_NW, session.MapPosition, trackElement, ride, session.CurrentRotation);
        track_paint_util_draw_station_covers(session, EDGE_NW, hasFence, stationObj, height);
    }
    else
    {
        hasFence = track_paint_util_has_fence(EDGE_NE, session.MapPosition, trackElement, ride, session.CurrentRotation);
        track_paint_util_draw_station_covers(session, EDGE_NE, hasFence, stationObj, height);
    }

    if (direction == 0 || direction == 2)
    {
        hasFence = track_paint_util_has_fence(EDGE_SE, session.MapPosition, trackElement, ride, session.CurrentRotation);
        track_paint_util_draw_station_covers(session, EDGE_SE, hasFence, stationObj, height);
    }
    else
    {
        hasFence = track_paint_util_has_fence(EDGE_SW, session.MapPosition, trackElement, ride, session.CurrentRotation);
        track_paint_util_draw_station_covers(session, EDGE_SW, hasFence, stationObj, height);
    }

    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

/** rct2: 0x00792D98 */
static void multi_dimension_rc_track_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15908, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15909, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15910, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15911, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            if (track_paint_util_should_paint_supports(session.MapPosition))
            {
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15880, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15881, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15882, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15883, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            if (track_paint_util_should_paint_supports(session.MapPosition))
            {
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
            }
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_SQUARE_8);
        }
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26253, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 40);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26254, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 40);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26255, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 40);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26256, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 40);
                break;
        }

        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            switch (direction)
            {
                case 0:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height + 54, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height + 54, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height + 54, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height + 54, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
        }

        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_SQUARE_8);
        }
        paint_util_set_general_support_height(session, height + 56, 0x20);
    }
}

/** rct2: 0x00792DA8 */
static void multi_dimension_rc_track_60_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15896, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15897, 0, 0, 32, 1, 98, height, 0, 27, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15898, 0, 0, 32, 1, 98, height, 0, 27, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15899, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 24, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 56, TUNNEL_SQUARE_8);
        }
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 104, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26269, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 88);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26270, 0, 0, 32, 2, 81, height + 24, 0, 4,
                    height + 6);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26271, 0, 0, 32, 2, 81, height + 24, 0, 4,
                    height + 6);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26272, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 88);
                break;
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 56, TUNNEL_SQUARE_8);
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 104, 0x20);
    }
}

/** rct2: 0x00792DB8 */
static void multi_dimension_rc_track_flat_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15900, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15901, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15902, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15903, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            if (track_paint_util_should_paint_supports(session.MapPosition))
            {
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 3, height, session.TrackColours[SCHEME_SUPPORTS]);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15872, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15873, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15874, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15875, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            if (track_paint_util_should_paint_supports(session.MapPosition))
            {
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 3, height, session.TrackColours[SCHEME_SUPPORTS]);
            }
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_8);
        }
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26245, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26246, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26247, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26248, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
        }

        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            switch (direction)
            {
                case 0:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height + 46, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height + 46, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height + 46, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height + 46, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
        }

        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_8);
        }
        paint_util_set_general_support_height(session, height + 48, 0x20);
    }
}

/** rct2: 0x00792DC8 */
static void multi_dimension_rc_track_25_deg_up_to_60_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15884, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15885, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15888, 0, 0, 32, 1, 66, height, 0, 27, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15886, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15889, 0, 0, 32, 1, 66, height, 0, 27, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15887, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 24, TUNNEL_SQUARE_8);
        }
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26257, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 56);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26261, 0, 0, 32, 10, 49, height + 24, 0, 10,
                    height + 6);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26258, 0, 0, 32, 2, 49, height + 24, 0, 4,
                    height + 6);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26262, 0, 0, 32, 10, 49, height + 24, 0, 10,
                    height + 6);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26259, 0, 0, 32, 2, 49, height + 24, 0, 4,
                    height + 6);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26260, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 56);
                break;
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 24, TUNNEL_SQUARE_8);
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
    }
}

/** rct2: 0x00792DD8 */
static void multi_dimension_rc_track_60_deg_up_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15890, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15891, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15894, 0, 0, 32, 1, 66, height, 0, 27, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15892, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15895, 0, 0, 32, 1, 66, height, 0, 27, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15893, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 16, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 24, TUNNEL_SQUARE_8);
        }
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26263, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 56);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26267, 0, 0, 32, 10, 49, height + 24, 0, 10,
                    height + 6);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26264, 0, 0, 32, 2, 49, height + 24, 0, 4,
                    height + 6);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26268, 0, 0, 32, 10, 49, height + 24, 0, 10,
                    height + 6);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26265, 0, 0, 32, 2, 49, height + 24, 0, 4,
                    height + 6);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26266, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 56);
                break;
        }

        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            switch (direction)
            {
                case 0:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height + 68, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height + 68, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height + 68, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height + 68, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
        }

        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 24, TUNNEL_SQUARE_8);
        }
        paint_util_set_general_support_height(session, height + 72, 0x20);
    }
}

/** rct2: 0x00792DE8 */
static void multi_dimension_rc_track_25_deg_up_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15904, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15905, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15906, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15907, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            if (track_paint_util_should_paint_supports(session.MapPosition))
            {
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 6, height, session.TrackColours[SCHEME_SUPPORTS]);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15876, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15877, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15878, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 15879, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            if (track_paint_util_should_paint_supports(session.MapPosition))
            {
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 6, height, session.TrackColours[SCHEME_SUPPORTS]);
            }
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_14);
        }
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 40, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26249, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26250, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26251, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26252, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
        }

        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            switch (direction)
            {
                case 0:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
        }

        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_14);
        }
        paint_util_set_general_support_height(session, height + 40, 0x20);
    }
}

/** rct2: 0x00792DF8 */
static void multi_dimension_rc_track_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_25_deg_up(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00792E08 */
static void multi_dimension_rc_track_60_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_60_deg_up(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00792E18 */
static void multi_dimension_rc_track_flat_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_25_deg_up_to_flat(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00792E28 */
static void multi_dimension_rc_track_25_deg_down_to_60_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_60_deg_up_to_25_deg_up(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00792E38 */
static void multi_dimension_rc_track_60_deg_down_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_25_deg_up_to_60_deg_up(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00792E48 */
static void multi_dimension_rc_track_25_deg_down_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_flat_to_25_deg_up(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00792E58 */
static void multi_dimension_rc_track_left_quarter_turn_5(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15921, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15926, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15931, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15916, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15920, 0, 0, 32, 16, 3, height, 0, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15925, 0, 0, 32, 16, 3, height, 0, 0,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15930, 0, 0, 32, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15915, 0, 0, 32, 16, 3, height, 0, 16,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15919, 0, 0, 16, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15924, 0, 0, 16, 16, 3, height, 16, 16,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15929, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15914, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15918, 0, 0, 16, 32, 3, height, 16, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15923, 0, 0, 16, 32, 3, height, 0, 0,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15928, 0, 0, 16, 32, 3, height, 0, 0,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15913, 0, 0, 16, 32, 3, height, 16, 0,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15917, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15922, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15927, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15912, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                switch (direction)
                {
                    case 2:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 3:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26310, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26315, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26320, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26305, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26309, 0, 0, 32, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26314, 0, 0, 32, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26319, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26304, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26308, 0, 0, 16, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26313, 0, 0, 16, 16, 3, height + 24, 16,
                            16, height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26318, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26303, 0, 0, 16, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26307, 0, 0, 16, 32, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26312, 0, 0, 16, 32, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26317, 0, 0, 16, 32, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26302, 0, 0, 16, 32, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26306, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26311, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26316, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26301, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D4, direction),
                    0xFFFF, 0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                switch (direction)
                {
                    case 2:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 3:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00792E68 */
static void multi_dimension_rc_track_right_quarter_turn_5(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
    multi_dimension_rc_track_left_quarter_turn_5(session, ride, trackSequence, (direction - 1) & 3, height, trackElement);
}

/** rct2: 0x00792E78 */
static void multi_dimension_rc_track_flat_to_left_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15832, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15840, 0, 0, 32, 1, 26, height, 0, 27, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15833, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15841, 0, 0, 32, 1, 26, height, 0, 27, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15834, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15835, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26273, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26274, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26275, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26276, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
        }

        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
            0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
        }

        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
}

/** rct2: 0x00792E88 */
static void multi_dimension_rc_track_flat_to_right_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15836, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15837, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15838, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15842, 0, 0, 32, 1, 26, height, 0, 27, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15839, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15843, 0, 0, 32, 1, 26, height, 0, 27, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26277, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26278, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26279, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26280, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
        }

        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
            0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
        }

        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
}

/** rct2: 0x00792E98 */
static void multi_dimension_rc_track_left_bank_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15838, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15842, 0, 0, 32, 1, 26, height, 0, 27, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15839, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15843, 0, 0, 32, 1, 26, height, 0, 27, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15836, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15837, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26279, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26280, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26277, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26278, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
        }

        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
            0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
        }

        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
}

/** rct2: 0x00792EA8 */
static void multi_dimension_rc_track_right_bank_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15834, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15835, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15832, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15840, 0, 0, 32, 1, 26, height, 0, 27, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15833, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15841, 0, 0, 32, 1, 26, height, 0, 27, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26275, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26276, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26273, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26274, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
        }

        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
            0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
        }

        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
}

/** rct2: 0x00792EB8 */
static void multi_dimension_rc_track_banked_left_quarter_turn_5(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15941, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15952, 0, 0, 32, 1, 26, height, 0, 27,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15946, 0, 0, 32, 1, 26, height, 0, 27,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15951, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15936, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15940, 0, 0, 32, 16, 3, height, 0, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15945, 0, 0, 32, 16, 1, height, 0, 0,
                            height + 27);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15950, 0, 0, 32, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15935, 0, 0, 32, 16, 3, height, 0, 16,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15939, 0, 0, 16, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15944, 0, 0, 16, 16, 1, height, 16, 16,
                            height + 27);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15949, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15934, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15938, 0, 0, 16, 32, 3, height, 16, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15943, 0, 0, 16, 32, 1, height, 0, 0,
                            height + 27);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15948, 0, 0, 16, 32, 3, height, 0, 0,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15933, 0, 0, 16, 32, 3, height, 16, 0,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15937, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15942, 0, 0, 1, 32, 26, height, 27, 0,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15947, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15953, 0, 0, 1, 32, 26, height, 27, 0,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15932, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                switch (direction)
                {
                    case 2:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 3:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26354, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 24);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26359, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 24);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26364, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 24);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26349, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 24);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26353, 0, 0, 32, 16, 3, height + 24);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26358, 0, 0, 32, 16, 3, height + 24);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26363, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 24);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26348, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 24);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26352, 0, 0, 16, 16, 3, height + 24, 0, 16,
                            height + 24);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26357, 0, 0, 16, 16, 3, height + 24, 16,
                            16, height + 24);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26362, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 24);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26347, 0, 0, 16, 16, 3, height + 24);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26351, 0, 0, 16, 32, 3, height + 24, 16, 0,
                            height + 24);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26356, 0, 0, 16, 32, 3, height + 24);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26361, 0, 0, 16, 32, 3, height + 24);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26346, 0, 0, 16, 32, 3, height + 24, 16, 0,
                            height + 24);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26350, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 24);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26355, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 24);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26360, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 24);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26345, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 24);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D4, direction),
                    0xFFFF, 0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                switch (direction)
                {
                    case 2:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 3:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00792EC8 */
static void multi_dimension_rc_track_banked_right_quarter_turn_5(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
    multi_dimension_rc_track_banked_left_quarter_turn_5(
        session, ride, trackSequence, (direction - 1) & 3, height, trackElement);
}

/** rct2: 0x00792ED8 */
static void multi_dimension_rc_track_left_bank_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15844, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15848, 0, 0, 32, 1, 34, height, 0, 27, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15845, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15849, 0, 0, 32, 1, 34, height, 0, 27, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15846, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15847, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 3, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_8);
        }
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26281, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26282, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26283, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26284, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
        }

        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
            0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            switch (direction)
            {
                case 0:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
        }

        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_8);
        }
        paint_util_set_general_support_height(session, height + 48, 0x20);
    }
}

/** rct2: 0x00792EE8 */
static void multi_dimension_rc_track_right_bank_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15850, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15851, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15852, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15854, 0, 0, 32, 1, 34, height, 0, 27, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15853, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15855, 0, 0, 32, 1, 34, height, 0, 27, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 3, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_8);
        }
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26285, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26286, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26287, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26288, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
        }

        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
            0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            switch (direction)
            {
                case 0:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
        }

        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_8);
        }
        paint_util_set_general_support_height(session, height + 48, 0x20);
    }
}

/** rct2: 0x00792EF8 */
static void multi_dimension_rc_track_25_deg_up_to_left_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15856, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15860, 0, 0, 32, 1, 34, height, 0, 27, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15857, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15861, 0, 0, 32, 1, 34, height, 0, 27, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15858, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15859, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 6, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_14);
        }
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 40, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26289, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26290, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26291, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26292, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
        }

        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
            0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            switch (direction)
            {
                case 0:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
        }

        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_14);
        }
        paint_util_set_general_support_height(session, height + 40, 0x20);
    }
}

/** rct2: 0x00792F08 */
static void multi_dimension_rc_track_25_deg_up_to_right_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15862, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15863, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15864, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15866, 0, 0, 32, 1, 34, height, 0, 27, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15865, 0, 0, 32, 20, 3, height, 0, 6, height);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15867, 0, 0, 32, 1, 34, height, 0, 27, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 6, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_14);
        }
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 40, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26293, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26294, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26295, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26296, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 32);
                break;
        }

        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
            0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            switch (direction)
            {
                case 0:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 1:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 2:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
                case 3:
                    metal_a_supports_paint_setup(
                        session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height + 44, session.TrackColours[SCHEME_SUPPORTS]);
                    break;
            }
        }

        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_FLAT);
        }
        else
        {
            paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_14);
        }
        paint_util_set_general_support_height(session, height + 40, 0x20);
    }
}

/** rct2: 0x00792F18 */
static void multi_dimension_rc_track_left_bank_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_25_deg_up_to_right_bank(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00792F28 */
static void multi_dimension_rc_track_right_bank_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_25_deg_up_to_left_bank(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00792F38 */
static void multi_dimension_rc_track_25_deg_down_to_left_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_right_bank_to_25_deg_up(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00792F48 */
static void multi_dimension_rc_track_25_deg_down_to_right_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_left_bank_to_25_deg_up(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00792F58 */
static void multi_dimension_rc_track_left_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15868, 0, 0, 32, 1, 26, height, 0, 27, height);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15869, 0, 0, 32, 1, 26, height, 0, 27, height);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15870, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15871, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26297, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26298, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26299, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26300, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 22);
                break;
        }

        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
            0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
        }

        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
}

/** rct2: 0x00792F68 */
static void multi_dimension_rc_track_right_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_left_bank(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00792F78 */
static void multi_dimension_rc_track_s_bend_left(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15816, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15820, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15819, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15823, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15817, 0, 0, 32, 26, 3, height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15821, 0, 0, 32, 26, 3, height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 6, 1, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15818, 0, 0, 32, 26, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15822, 0, 0, 32, 26, 3, height, 0, 6,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15818, 0, 0, 32, 26, 3, height, 0, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15822, 0, 0, 32, 26, 3, height, 0, 6,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15817, 0, 0, 32, 26, 3, height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15821, 0, 0, 32, 26, 3, height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 6, 1, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15819, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15823, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15816, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15820, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                switch (direction)
                {
                    case 1:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 2:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26229, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26233, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26232, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26236, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF,
                    0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26230, 0, 0, 32, 26, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26234, 0, 0, 32, 26, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26231, 0, 0, 32, 26, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26235, 0, 0, 32, 26, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26231, 0, 0, 32, 26, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26235, 0, 0, 32, 26, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26230, 0, 0, 32, 26, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26234, 0, 0, 32, 26, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26232, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26236, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26229, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26233, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF,
                    0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                switch (direction)
                {
                    case 1:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 2:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00792F88 */
static void multi_dimension_rc_track_s_bend_right(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15824, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15828, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15827, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15831, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15825, 0, 0, 32, 26, 3, height, 0, 6,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15829, 0, 0, 32, 26, 3, height, 0, 6,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15826, 0, 0, 32, 26, 3, height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15830, 0, 0, 32, 26, 3, height);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15826, 0, 0, 32, 26, 3, height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15830, 0, 0, 32, 26, 3, height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15825, 0, 0, 32, 26, 3, height, 0, 6,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15829, 0, 0, 32, 26, 3, height, 0, 6,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15827, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15831, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15824, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15828, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                switch (direction)
                {
                    case 1:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 2:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26237, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26241, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26240, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26244, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF,
                    0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26238, 0, 0, 32, 26, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26242, 0, 0, 32, 26, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26239, 0, 0, 32, 26, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26243, 0, 0, 32, 26, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26239, 0, 0, 32, 26, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26243, 0, 0, 32, 26, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26238, 0, 0, 32, 26, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26242, 0, 0, 32, 26, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26240, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26244, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26237, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26241, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF,
                    0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                switch (direction)
                {
                    case 1:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 2:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00792FC8 */
static void multi_dimension_rc_track_left_quarter_turn_3(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15959, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15962, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15965, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15956, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15958, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15961, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15964, 0, 0, 16, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15955, 0, 0, 16, 16, 3, height, 16, 16,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15957, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15960, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15963, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15954, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                switch (direction)
                {
                    case 2:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 3:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26326, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26329, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26332, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26323, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26325, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26328, 0, 0, 16, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26331, 0, 0, 16, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26322, 0, 0, 16, 16, 3, height + 24, 16,
                            16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26324, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26327, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26330, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26321, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D4, direction),
                    0xFFFF, 0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                switch (direction)
                {
                    case 2:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 3:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00792FD8 */
static void multi_dimension_rc_track_right_quarter_turn_3(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
    multi_dimension_rc_track_left_quarter_turn_3(session, ride, trackSequence, (direction - 1) & 3, height, trackElement);
}

/** rct2: 0x00792FE8 */
static void multi_dimension_rc_track_left_quarter_turn_3_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15971, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15978, 0, 0, 32, 1, 26, height, 0, 27,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15974, 0, 0, 32, 1, 26, height, 0, 27,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15977, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15968, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15970, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15973, 0, 0, 16, 16, 1, height, 0, 0,
                            height + 27);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15976, 0, 0, 16, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15967, 0, 0, 16, 16, 3, height, 16, 16,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15969, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15972, 0, 0, 1, 32, 26, height, 27, 0,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15975, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15979, 0, 0, 1, 32, 26, height, 27, 0,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15966, 0, 0, 20, 32, 3, height, 6, 0,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                switch (direction)
                {
                    case 2:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 3:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26338, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26341, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26344, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26335, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26337, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26340, 0, 0, 16, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26343, 0, 0, 16, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26334, 0, 0, 16, 16, 3, height + 24, 16,
                            16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26336, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26339, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26342, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26333, 0, 0, 20, 32, 3, height + 24, 6, 0,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D4, direction),
                    0xFFFF, 0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                switch (direction)
                {
                    case 2:
                        paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                    case 3:
                        paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                        break;
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00792FF8 */
static void multi_dimension_rc_track_right_quarter_turn_3_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
    multi_dimension_rc_track_left_quarter_turn_3_bank(session, ride, trackSequence, (direction - 1) & 3, height, trackElement);
}

/** rct2: 0x00793008 */
static void multi_dimension_rc_track_left_half_banked_helix_up_small(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16197, 0, 0, 32, 20, 3, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16204, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16200, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16203, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16194, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 2, height, session.TrackColours[SCHEME_SUPPORTS]);
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16196, 0, 0, 16, 16, 3, height, 16, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16199, 0, 0, 16, 16, 1, height, 0, 0,
                        height + 27);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16202, 0, 0, 16, 16, 3, height, 0, 16, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16193, 0, 0, 16, 16, 3, height, 16, 16,
                        height);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16195, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16198, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16201, 0, 0, 20, 32, 3, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16205, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16192, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 6, height, session.TrackColours[SCHEME_SUPPORTS]);
            switch (direction)
            {
                case 2:
                    paint_util_push_tunnel_right(session, height + 8, TUNNEL_SQUARE_FLAT);
                    break;
                case 3:
                    paint_util_push_tunnel_left(session, height + 8, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 4:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16194, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16197, 0, 0, 20, 32, 3, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16204, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16200, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16203, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 2, height, session.TrackColours[SCHEME_SUPPORTS]);
            switch (direction)
            {
                case 0:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 1:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 5:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 6:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16193, 0, 0, 16, 16, 3, height, 16, 16,
                        height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16196, 0, 0, 16, 16, 3, height, 0, 16, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16199, 0, 0, 16, 16, 1, height, 0, 0,
                        height + 27);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16202, 0, 0, 16, 16, 3, height, 16, 0, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 7:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16192, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16195, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16198, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16201, 0, 0, 32, 20, 3, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16205, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 6, height, session.TrackColours[SCHEME_SUPPORTS]);
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x00793018 */
static void multi_dimension_rc_track_right_half_banked_helix_up_small(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16178, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16181, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16184, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16187, 0, 0, 32, 20, 3, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16191, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 2, height, session.TrackColours[SCHEME_SUPPORTS]);
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16179, 0, 0, 16, 16, 3, height, 16, 16,
                        height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16182, 0, 0, 16, 16, 3, height, 0, 16, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16185, 0, 0, 16, 16, 1, height, 0, 0,
                        height + 27);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16188, 0, 0, 16, 16, 3, height, 16, 0, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16180, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16183, 0, 0, 20, 32, 3, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16190, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16186, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16189, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 6, height, session.TrackColours[SCHEME_SUPPORTS]);
            switch (direction)
            {
                case 0:
                    paint_util_push_tunnel_right(session, height + 8, TUNNEL_SQUARE_FLAT);
                    break;
                case 1:
                    paint_util_push_tunnel_left(session, height + 8, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 4:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16181, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16184, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16187, 0, 0, 20, 32, 3, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16191, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16178, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 2, height, session.TrackColours[SCHEME_SUPPORTS]);
            switch (direction)
            {
                case 2:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 3:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 5:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 6:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16182, 0, 0, 16, 16, 3, height, 16, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16185, 0, 0, 16, 16, 1, height, 0, 0,
                        height + 27);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16188, 0, 0, 16, 16, 3, height, 0, 16, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16179, 0, 0, 16, 16, 3, height, 16, 16,
                        height);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 7:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16183, 0, 0, 32, 20, 3, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16190, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16186, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16189, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16180, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 6, height, session.TrackColours[SCHEME_SUPPORTS]);
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x00793028 */
static void multi_dimension_rc_track_left_half_banked_helix_down_small(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (trackSequence >= 4)
    {
        trackSequence -= 4;
        direction = (direction - 1) & 3;
    }
    trackSequence = mapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
    multi_dimension_rc_track_right_half_banked_helix_up_small(
        session, ride, trackSequence, (direction + 1) & 3, height, trackElement);
}

/** rct2: 0x00793038 */
static void multi_dimension_rc_track_right_half_banked_helix_down_small(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (trackSequence >= 4)
    {
        trackSequence -= 4;
        direction = (direction + 1) & 3;
    }
    trackSequence = mapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
    multi_dimension_rc_track_left_half_banked_helix_up_small(
        session, ride, trackSequence, (direction - 1) & 3, height, trackElement);
}

/** rct2: 0x00793048 */
static void multi_dimension_rc_track_left_half_banked_helix_up_large(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16165, 0, 0, 32, 20, 3, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16176, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16170, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16175, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16160, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 1, height, session.TrackColours[SCHEME_SUPPORTS]);
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16164, 0, 0, 32, 16, 3, height, 0, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16169, 0, 0, 32, 16, 1, height, 0, 0,
                        height + 27);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16174, 0, 0, 32, 16, 3, height, 0, 16, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16159, 0, 0, 32, 16, 3, height, 0, 16, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16163, 0, 0, 16, 16, 3, height, 0, 16, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16168, 0, 0, 16, 16, 1, height, 16, 16,
                        height + 27);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16173, 0, 0, 16, 16, 3, height, 16, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16158, 0, 0, 16, 16, 3, height, 0, 0, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 4:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 5:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16162, 0, 0, 16, 32, 3, height, 16, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16167, 0, 0, 16, 32, 1, height, 0, 0,
                        height + 27);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16172, 0, 0, 16, 32, 3, height, 0, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16157, 0, 0, 16, 32, 3, height, 16, 0, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 6:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16161, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16166, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16171, 0, 0, 20, 32, 3, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16177, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16156, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 7, height, session.TrackColours[SCHEME_SUPPORTS]);
            switch (direction)
            {
                case 2:
                    paint_util_push_tunnel_right(session, height + 8, TUNNEL_SQUARE_FLAT);
                    break;
                case 3:
                    paint_util_push_tunnel_left(session, height + 8, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 7:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16160, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16165, 0, 0, 20, 32, 3, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16176, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16170, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16175, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 1, height, session.TrackColours[SCHEME_SUPPORTS]);
            switch (direction)
            {
                case 0:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 1:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 8:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 9:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16159, 0, 0, 16, 32, 3, height, 16, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16164, 0, 0, 16, 32, 3, height, 0, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16169, 0, 0, 16, 32, 1, height, 0, 0,
                        height + 27);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16174, 0, 0, 16, 32, 3, height, 16, 0, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 10:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16158, 0, 0, 16, 16, 3, height, 0, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16163, 0, 0, 16, 16, 3, height, 16, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16168, 0, 0, 16, 16, 1, height, 16, 16,
                        height + 27);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16173, 0, 0, 16, 16, 3, height, 0, 16, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 11:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 12:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16157, 0, 0, 32, 16, 3, height, 0, 16, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16162, 0, 0, 32, 16, 3, height, 0, 16, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16167, 0, 0, 32, 16, 1, height, 0, 0,
                        height + 27);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16172, 0, 0, 32, 16, 3, height, 0, 0, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 13:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16156, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16161, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16166, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16171, 0, 0, 32, 20, 3, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16177, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 7, height, session.TrackColours[SCHEME_SUPPORTS]);
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x00793058 */
static void multi_dimension_rc_track_right_half_banked_helix_up_large(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16134, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16139, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16144, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16149, 0, 0, 32, 20, 3, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16155, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 1, height, session.TrackColours[SCHEME_SUPPORTS]);
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16135, 0, 0, 32, 16, 3, height, 0, 16, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16140, 0, 0, 32, 16, 3, height, 0, 16, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16145, 0, 0, 32, 16, 1, height, 0, 0,
                        height + 27);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16150, 0, 0, 32, 16, 3, height, 0, 0, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 3:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16136, 0, 0, 16, 16, 3, height, 0, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16141, 0, 0, 16, 16, 3, height, 16, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16146, 0, 0, 16, 16, 1, height, 16, 16,
                        height + 27);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16151, 0, 0, 16, 16, 3, height, 0, 16, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 4:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 5:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16137, 0, 0, 16, 32, 3, height, 16, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16142, 0, 0, 16, 32, 3, height, 0, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16147, 0, 0, 16, 32, 1, height, 0, 0,
                        height + 27);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16152, 0, 0, 16, 32, 3, height, 16, 0, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 6:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16138, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16143, 0, 0, 20, 32, 3, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16154, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16148, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16153, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 7, height, session.TrackColours[SCHEME_SUPPORTS]);
            switch (direction)
            {
                case 0:
                    paint_util_push_tunnel_right(session, height + 8, TUNNEL_SQUARE_FLAT);
                    break;
                case 1:
                    paint_util_push_tunnel_left(session, height + 8, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 7:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16139, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16144, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16149, 0, 0, 20, 32, 3, height, 6, 0, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16155, 0, 0, 1, 32, 26, height, 27, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16134, 0, 0, 20, 32, 3, height, 6, 0, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 1, height, session.TrackColours[SCHEME_SUPPORTS]);
            switch (direction)
            {
                case 2:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 3:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 8:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 9:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16140, 0, 0, 16, 32, 3, height, 16, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16145, 0, 0, 16, 32, 1, height, 0, 0,
                        height + 27);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16150, 0, 0, 16, 32, 3, height, 0, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16135, 0, 0, 16, 32, 3, height, 16, 0, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 10:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16141, 0, 0, 16, 16, 3, height, 0, 16, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16146, 0, 0, 16, 16, 1, height, 16, 16,
                        height + 27);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16151, 0, 0, 16, 16, 3, height, 16, 0, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16136, 0, 0, 16, 16, 3, height, 0, 0, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 11:
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 12:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16142, 0, 0, 32, 16, 3, height, 0, 0, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16147, 0, 0, 32, 16, 1, height, 0, 0,
                        height + 27);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16152, 0, 0, 32, 16, 3, height, 0, 16, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16137, 0, 0, 32, 16, 3, height, 0, 16, height);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 13:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16143, 0, 0, 32, 20, 3, height, 0, 6, height);
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16154, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16148, 0, 0, 32, 1, 26, height, 0, 27, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16153, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 16138, 0, 0, 32, 20, 3, height, 0, 6, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 7, height, session.TrackColours[SCHEME_SUPPORTS]);
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x00793068 */
static void multi_dimension_rc_track_left_half_banked_helix_down_large(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (trackSequence >= 7)
    {
        trackSequence -= 7;
        direction = (direction - 1) & 3;
    }
    trackSequence = mapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
    multi_dimension_rc_track_right_half_banked_helix_up_large(
        session, ride, trackSequence, (direction + 1) & 3, height, trackElement);
}

/** rct2: 0x00793078 */
static void multi_dimension_rc_track_right_half_banked_helix_down_large(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (trackSequence >= 7)
    {
        trackSequence -= 7;
        direction = (direction + 1) & 3;
    }
    trackSequence = mapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
    multi_dimension_rc_track_left_half_banked_helix_up_large(
        session, ride, trackSequence, (direction - 1) & 3, height, trackElement);
}

/** rct2: 0x00793088 */
static void multi_dimension_rc_track_brakes(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 16218, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
            case 1:
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 16219, 0, 0, 32, 20, 3, height, 0, 6, height);
                break;
        }
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
        }
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26549, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 24);
                break;
            case 1:
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26550, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 24);
                break;
        }

        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
        }

        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
}

/** rct2: 0x00793098 */
static void multi_dimension_rc_track_on_ride_photo(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, IMAGE_TYPE_REMAP | SPR_STATION_BASE_D, 0, 0, 32, 32, 1, height);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15806, 0, 0, 32, 20, 0, height, 0, 6, height + 3);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, IMAGE_TYPE_REMAP | SPR_STATION_BASE_D, 0, 0, 32, 32, 1, height);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15807, 0, 0, 32, 20, 0, height, 0, 6, height + 3);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, IMAGE_TYPE_REMAP | SPR_STATION_BASE_D, 0, 0, 32, 32, 1, height);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15806, 0, 0, 32, 20, 0, height, 0, 6, height + 3);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, IMAGE_TYPE_REMAP | SPR_STATION_BASE_D, 0, 0, 32, 32, 1, height);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 15807, 0, 0, 32, 20, 0, height, 0, 6, height + 3);
                break;
        }
        track_paint_util_onride_photo_paint(session, direction, height + 3, trackElement);
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, IMAGE_TYPE_REMAP | SPR_STATION_BASE_D, 0, 0, 32, 32, 1, height);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26227, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 24);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, IMAGE_TYPE_REMAP | SPR_STATION_BASE_D, 0, 0, 32, 32, 1, height);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26228, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 24);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, IMAGE_TYPE_REMAP | SPR_STATION_BASE_D, 0, 0, 32, 32, 1, height);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 5, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 8, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26227, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 24);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, IMAGE_TYPE_REMAP | SPR_STATION_BASE_D, 0, 0, 32, 32, 1, height);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 6, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 7, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26228, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 24);
                break;
        }
        track_paint_util_onride_photo_paint(session, direction, height + 3, trackElement);
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 64, 0x20);
    }
}

/** rct2: 0x00793328 */
static void multi_dimension_rc_track_90_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16130, 0, 0, 2, 20, 31, height, 4, 6,
                            height + 8);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16131, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16132, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16133, 0, 0, 2, 20, 31, height, 4, 6,
                            height + 8);
                        break;
                }
                paint_util_set_vertical_tunnel(session, height + 32);
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26533, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 56);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26534, 0, 0, 32, 2, 31, height + 24, 0, 4,
                            height + 6);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26535, 0, 0, 32, 2, 31, height + 24, 0, 4,
                            height + 6);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26536, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 56);
                        break;
                }
                paint_util_set_vertical_tunnel(session, height + 32);
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                break;
        }
    }
}

/** rct2: 0x00793338 */
static void multi_dimension_rc_track_90_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_90_deg_up(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00793348 */
static void multi_dimension_rc_track_60_deg_up_to_90_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16122, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16123, 0, 0, 2, 20, 55, height, 24, 6,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16124, 0, 0, 2, 20, 55, height, 24, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16125, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_SQUARE_7);
                }
                paint_util_set_vertical_tunnel(session, height + 56);
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 1:
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26525, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 80);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26526, 0, 0, 32, 2, 55, height + 24, 0, 4,
                            height + 6);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26527, 0, 0, 32, 2, 55, height + 24, 0, 4,
                            height + 6);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26528, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 80);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_INVERTED_4);
                }
                paint_util_set_vertical_tunnel(session, height + 56);
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 1:
                break;
        }
    }
}

/** rct2: 0x00793358 */
static void multi_dimension_rc_track_90_deg_down_to_60_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    multi_dimension_rc_track_60_deg_up_to_90_deg_up(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00793368 */
static void multi_dimension_rc_track_90_deg_up_to_60_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 16126, 0, 0, 6, 20, 3, height, 0, 6, height + 2);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 16127, 0, 0, 2, 20, 31, height, 39, 6, height + 8);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 16128, 0, 0, 2, 20, 31, height, 39, 6, height + 8);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 16129, 0, 0, 6, 20, 3, height, 0, 6, height + 2);
                break;
        }
        switch (direction)
        {
            case 1:
                paint_util_push_tunnel_right(session, height + 48, TUNNEL_SQUARE_8);
                break;
            case 2:
                paint_util_push_tunnel_left(session, height + 48, TUNNEL_SQUARE_8);
                break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 80, 0x20);
    }
    else
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26529, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 80);
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26530, 0, 0, 32, 2, 55, height + 24, 0, 4,
                    height + 6);
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26531, 0, 0, 32, 2, 55, height + 24, 0, 4,
                    height + 6);
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours[SCHEME_TRACK] | 26532, 0, 0, 32, 20, 3, height + 24, 0, 6,
                    height + 80);
                break;
        }
        switch (direction)
        {
            case 1:
                paint_util_push_tunnel_right(session, height + 48, TUNNEL_INVERTED_5);
                break;
            case 2:
                paint_util_push_tunnel_left(session, height + 48, TUNNEL_INVERTED_5);
                break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 96, 0x20);
    }
}

/** rct2: 0x00793378 */
static void multi_dimension_rc_track_60_deg_down_to_90_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16128, 0, 0, 2, 20, 31, height, 39, 6,
                            height + 8);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16129, 0, 0, 6, 20, 3, height, 0, 6,
                            height + 2);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16126, 0, 0, 6, 20, 3, height, 0, 6,
                            height + 2);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16127, 0, 0, 2, 20, 31, height, 39, 6,
                            height + 8);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height + 48, TUNNEL_SQUARE_8);
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 80, 0x20);
                break;
            case 1:
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26531, 0, 0, 32, 2, 55, height + 24, 0, 4,
                            height + 6);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26532, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 80);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26529, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 80);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26530, 0, 0, 32, 2, 55, height + 24, 0, 4,
                            height + 6);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height + 48, TUNNEL_INVERTED_5);
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 96, 0x20);
                break;
            case 1:
                break;
        }
    }
}

/** rct2: 0x007930B8 */
static void multi_dimension_rc_track_left_eighth_to_diag(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15996, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16000, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16004, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16008, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15997, 0, 0, 32, 16, 3, height, 0, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16001, 0, 0, 34, 16, 3, height, 0, 0,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16005, 0, 0, 32, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16009, 0, 0, 32, 16, 3, height, 0, 16,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15998, 0, 0, 16, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16002, 0, 0, 16, 16, 3, height, 16, 16,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16006, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16010, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15999, 0, 0, 16, 16, 3, height, 16, 16,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16003, 0, 0, 16, 18, 3, height, 0, 16,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16007, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16011, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26405, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26409, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26413, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26417, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26406, 0, 0, 32, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26410, 0, 0, 34, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26414, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26418, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26407, 0, 0, 16, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26411, 0, 0, 16, 16, 3, height + 24, 16,
                            16, height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26415, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26419, 0, 0, 16, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26408, 0, 0, 16, 16, 3, height + 24, 16,
                            16, height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26412, 0, 0, 16, 18, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26416, 0, 0, 16, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26420, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x007930C8 */
static void multi_dimension_rc_track_right_eighth_to_diag(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15980, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15984, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15988, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15992, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15981, 0, 0, 32, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15985, 0, 0, 32, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15989, 0, 0, 34, 16, 3, height, 0, 0,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15993, 0, 0, 32, 16, 3, height, 0, 0,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15982, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15986, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15990, 0, 0, 28, 28, 3, height, 4, 4,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15994, 0, 0, 16, 16, 3, height, 0, 16,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15983, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15987, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15991, 0, 0, 16, 18, 3, height, 0, 16,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 15995, 0, 0, 16, 16, 3, height, 16, 16,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26389, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26393, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26397, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26401, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26390, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26394, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26398, 0, 0, 34, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26402, 0, 0, 32, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26391, 0, 0, 16, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26395, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26399, 0, 0, 28, 28, 3, height + 24, 4, 4,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26403, 0, 0, 16, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26392, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26396, 0, 0, 16, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26400, 0, 0, 16, 18, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26404, 0, 0, 16, 16, 3, height + 24, 16,
                            16, height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x007930D8 */
static void multi_dimension_rc_track_left_eighth_to_orthogonal(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
    multi_dimension_rc_track_right_eighth_to_diag(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x007930E8 */
static void multi_dimension_rc_track_right_eighth_to_orthogonal(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
    multi_dimension_rc_track_left_eighth_to_diag(session, ride, trackSequence, (direction + 3) & 3, height, trackElement);
}

/** rct2: 0x007930F8 */
static void multi_dimension_rc_track_left_eighth_bank_to_diag(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16028, 0, 0, 32, 1, 26, height, 0, 27,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16032, 0, 0, 32, 1, 26, height, 0, 27,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16036, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16040, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16029, 0, 0, 32, 16, 3, height, 0, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16033, 0, 0, 34, 16, 0, height, 0, 0,
                            height + 27);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16037, 0, 0, 32, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16041, 0, 0, 32, 16, 3, height, 0, 16,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16030, 0, 0, 16, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16034, 0, 0, 16, 16, 0, height, 16, 16,
                            height + 27);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16038, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16042, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16031, 0, 0, 16, 16, 3, height, 16, 16,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16035, 0, 0, 16, 18, 0, height, 0, 16,
                            height + 27);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16039, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16043, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26437, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26441, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26445, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26449, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26438, 0, 0, 32, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26442, 0, 0, 34, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26446, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26450, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26439, 0, 0, 16, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26443, 0, 0, 16, 16, 3, height + 24, 16,
                            16, height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26447, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26451, 0, 0, 16, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26440, 0, 0, 16, 16, 3, height + 24, 16,
                            16, height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26444, 0, 0, 16, 18, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26448, 0, 0, 16, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26452, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793108 */
static void multi_dimension_rc_track_right_eighth_bank_to_diag(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16012, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16016, 0, 0, 32, 20, 3, height, 0, 6,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16020, 0, 0, 32, 1, 26, height, 0, 27,
                            height);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16024, 0, 0, 32, 1, 26, height, 0, 27,
                            height);
                        break;
                }
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16013, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16017, 0, 0, 32, 16, 3, height, 0, 16,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16021, 0, 0, 34, 16, 0, height, 0, 0,
                            height + 27);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16025, 0, 0, 32, 16, 3, height, 0, 0,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16014, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16018, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16022, 0, 0, 28, 28, 0, height, 4, 4,
                            height + 27);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16026, 0, 0, 16, 16, 3, height, 0, 16,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16015, 0, 0, 16, 16, 3, height, 16, 0,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16019, 0, 0, 16, 16, 3, height, 0, 0,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16023, 0, 0, 16, 18, 0, height, 0, 16,
                            height + 27);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16027, 0, 0, 16, 16, 3, height, 16, 16,
                            height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26421, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26425, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26429, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26433, 0, 0, 32, 20, 3, height + 24, 0, 6,
                            height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF,
                    0);
                metal_a_supports_paint_setup(
                    session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26422, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26426, 0, 0, 32, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26430, 0, 0, 34, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26434, 0, 0, 32, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26423, 0, 0, 16, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26427, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26431, 0, 0, 28, 28, 3, height + 24, 4, 4,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26435, 0, 0, 16, 16, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction),
                    0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26424, 0, 0, 16, 16, 3, height + 24, 16, 0,
                            height + 22);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26428, 0, 0, 16, 16, 3, height + 24, 0, 0,
                            height + 22);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26432, 0, 0, 16, 18, 3, height + 24, 0, 16,
                            height + 22);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26436, 0, 0, 16, 16, 3, height + 24, 16,
                            16, height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(
                        SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793118 */
static void multi_dimension_rc_track_left_eighth_bank_to_orthogonal(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
    multi_dimension_rc_track_right_eighth_bank_to_diag(session, ride, trackSequence, (direction + 2) & 3, height, trackElement);
}

/** rct2: 0x00793128 */
static void multi_dimension_rc_track_right_eighth_bank_to_orthogonal(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
    multi_dimension_rc_track_left_eighth_bank_to_diag(session, ride, trackSequence, (direction + 3) & 3, height, trackElement);
}

/** rct2: 0x007930A8 */
static void multi_dimension_rc_track_diag_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16109, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16047, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16106, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16044, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16108, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16046, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            metal_a_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16107, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_a_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_a_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_a_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            metal_a_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16045, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_a_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_a_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_a_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26484, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 22);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26456, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 22);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26481, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 22);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26453, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 22);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26483, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 22);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26455, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 22);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26482, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 22);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26454, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 22);
                            break;
                    }
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793158 */
static void multi_dimension_rc_track_diag_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16121, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16059, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16118, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16056, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16120, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16058, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16119, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16057, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26496, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 40);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26468, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 40);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26493, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 40);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26465, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 40);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26495, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 40);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26467, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 40);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26494, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 40);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26466, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 40);
                            break;
                    }
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 48, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 48, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 48, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 48, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
        }
    }
}

/** rct2: 0x007931B8 */
static void multi_dimension_rc_track_diag_60_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16071, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16068, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16070, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 32, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16069, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 36, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 32, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 36, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26480, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 88);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26477, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 88);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26479, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 88);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26478, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 24);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 32, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 36, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 32, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 36, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793138 */
static void multi_dimension_rc_track_diag_flat_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16113, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16051, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16110, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16048, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16112, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16050, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16111, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16049, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26488, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26460, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26485, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26457, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26487, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26459, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26486, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26458, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793198 */
static void multi_dimension_rc_track_diag_25_deg_up_to_60_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16063, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16060, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16062, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 16, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16061, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 16, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 16, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 16, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26472, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26469, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26471, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26470, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 16, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 16, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 16, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 16, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
        }
    }
}

/** rct2: 0x007931A8 */
static void multi_dimension_rc_track_diag_60_deg_up_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16067, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16064, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16066, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 21, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16065, -16, -16, 16, 16, 3, height, 0, 0,
                            height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 21, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 21, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 21, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26476, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26473, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26475, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26474, -16, -16, 16, 16, 3, height + 24, 0,
                            0, height + 56);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 21, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 21, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 21, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 21, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793148 */
static void multi_dimension_rc_track_diag_25_deg_up_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16117, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16055, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16114, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16052, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16116, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16054, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16115, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16053, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26492, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26464, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26489, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26461, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26491, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26463, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26490, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 26462, -16, -16, 32, 32, 3,
                                height + 24, -16, -16, height + 32);
                            break;
                    }
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793188 */
static void multi_dimension_rc_track_diag_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16119, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16057, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16120, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16058, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16118, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16056, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16121, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16059, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26466, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 40);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26467, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 40);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26465, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 40);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26468, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 40);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 48, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 48, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 48, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 48, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
        }
    }
}

/** rct2: 0x007931E8 */
static void multi_dimension_rc_track_diag_60_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16069, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16070, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16068, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 24, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16071, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 28, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 24, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 28, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26478, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 24);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26479, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 88);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26477, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 88);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26480, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 88);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 24, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 28, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 24, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 28, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793168 */
static void multi_dimension_rc_track_diag_flat_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16115, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16053, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16116, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16054, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16114, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16052, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16117, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16055, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26462, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26463, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26461, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26464, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                break;
        }
    }

    paint_util_set_general_support_height(session, height + 56, 0x20);
}

/** rct2: 0x007931C8 */
static void multi_dimension_rc_track_diag_25_deg_down_to_60_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16065, -16, -16, 16, 16, 3, height, 0, 0,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16066, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16064, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 17, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16067, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 17, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 17, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 17, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26474, -16, -16, 16, 16, 3, height + 24, 0,
                            0, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26475, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26473, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26476, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 17, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 17, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 17, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 17, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
        }
    }
}

/** rct2: 0x007931D8 */
static void multi_dimension_rc_track_diag_60_deg_down_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16061, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16062, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16060, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16063, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 8, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26470, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26471, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26469, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26472, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 56);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 8, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 8, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 8, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 8, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793178 */
static void multi_dimension_rc_track_diag_25_deg_down_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16111, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16049, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16112, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16050, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16110, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16048, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16113, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction, session.TrackColours[SCHEME_TRACK] | 16051, -16, -16, 32, 32, 3, height,
                                -16, -16, height);
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 2:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                        case 3:
                            metal_b_supports_paint_setup(
                                session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                            break;
                    }
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26458, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26459, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26457, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26460, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793218 */
static void multi_dimension_rc_track_diag_flat_to_left_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16079, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16076, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16080, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 27);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16078, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16077, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26504, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26501, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26503, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26502, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793228 */
static void multi_dimension_rc_track_diag_flat_to_right_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16084, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16081, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16083, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16085, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 27);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16082, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26508, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26505, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26507, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26506, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793238 */
static void multi_dimension_rc_track_diag_left_bank_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16082, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16083, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16085, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 27);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16081, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16084, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26506, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26507, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26505, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26508, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793248 */
static void multi_dimension_rc_track_diag_right_bank_to_flat(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16077, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16078, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16076, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16080, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 27);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16079, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26502, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26503, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26501, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26504, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793278 */
static void multi_dimension_rc_track_diag_left_bank_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16099, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16096, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16100, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16098, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16097, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26520, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26517, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26519, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26518, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793288 */
static void multi_dimension_rc_track_diag_right_bank_to_25_deg_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16104, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16101, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16103, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16105, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16102, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26524, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26521, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26523, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26522, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793258 */
static void multi_dimension_rc_track_diag_25_deg_up_to_left_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16089, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16086, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16090, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16088, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16087, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26512, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26509, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26511, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26510, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793268 */
static void multi_dimension_rc_track_diag_25_deg_up_to_right_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16094, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16091, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16093, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16095, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16092, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26516, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26513, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26515, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26514, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 56, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793298 */
static void multi_dimension_rc_track_diag_left_bank_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16092, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16093, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16095, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16091, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16094, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26514, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26515, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26513, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26516, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                break;
        }
    }

    paint_util_set_general_support_height(session, height + 56, 0x20);
}

/** rct2: 0x007932A8 */
static void multi_dimension_rc_track_diag_right_bank_to_25_deg_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16087, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16088, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16086, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16090, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16089, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 4, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26510, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26511, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26509, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26512, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 42, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                break;
        }
    }

    paint_util_set_general_support_height(session, height + 56, 0x20);
}

/** rct2: 0x007932B8 */
static void multi_dimension_rc_track_diag_25_deg_down_to_left_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16102, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16103, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16105, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16101, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16104, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26522, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26523, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26521, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26524, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
}

/** rct2: 0x007932C8 */
static void multi_dimension_rc_track_diag_25_deg_down_to_right_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16097, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16098, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16096, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16100, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 35);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16099, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_b_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26518, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26519, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26517, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26520, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 32);
                        break;
                }

                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 40, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
}

/** rct2: 0x007931F8 */
static void multi_dimension_rc_track_diag_left_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16075, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16072, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 27);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16074, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16073, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26500, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26497, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26499, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26498, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793208 */
static void multi_dimension_rc_track_diag_right_bank(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16073, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16074, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16072, -16, -16, 32, 32, 0, height, -16,
                            -16, height + 27);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16075, -16, -16, 32, 32, 3, height, -16,
                            -16, height);
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26498, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26499, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26497, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF,
                    0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
            case 3:
                switch (direction)
                {
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26500, -16, -16, 32, 32, 3, height + 24,
                            -16, -16, height + 22);
                        break;
                }

                paint_util_set_segment_support_height(
                    session,
                    paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction),
                    0xFFFF, 0);
                switch (direction)
                {
                    case 0:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 1, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 1:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 0, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 2:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 2, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                    case 3:
                        metal_a_supports_paint_setup(
                            session, METAL_SUPPORTS_TUBES_INVERTED, 3, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
                        break;
                }

                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x007932D8 */
static void multi_dimension_rc_track_left_flyer_twist_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26368, 0, 6, 32, 20, 3, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26374, 0, 6, 32, 20, 3, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26367, 0, 6, 32, 20, 3, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26373, 0, 6, 32, 20, 3, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26369, 0, 6, 32, 20, 3, height + 24, 0, 6,
                        height + 24);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26375, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26366, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26372, 0, 6, 32, 20, 3, height + 24);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26370, 0, 6, 32, 20, 3, height + 24, 0, 6,
                        height + 24);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26376, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26365, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26371, 0, 6, 32, 20, 3, height + 24);
                    break;
            }

            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                0xFFFF, 0);
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

            switch (direction)
            {
                case 1:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 2:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x007932E8 */
static void multi_dimension_rc_track_right_flyer_twist_up(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26380, 0, 6, 32, 20, 3, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26386, 0, 6, 32, 20, 3, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26379, 0, 6, 32, 20, 3, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26385, 0, 6, 32, 20, 3, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26381, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26387, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26378, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26384, 0, 6, 32, 20, 3, height + 24);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26382, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26388, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26377, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26383, 0, 6, 32, 20, 3, height + 24);
                    break;
            }

            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

            switch (direction)
            {
                case 1:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 2:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x007932F8 */
static void multi_dimension_rc_track_left_flyer_twist_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26365, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26371, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26370, 0, 6, 32, 20, 3, height + 24, 0, 6,
                        height + 24);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26376, 0, 6, 32, 20, 3, height + 24);
                    break;
            }

            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26366, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26372, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26369, 0, 6, 32, 20, 3, height + 24, 0, 6,
                        height + 24);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26375, 0, 6, 32, 20, 3, height + 24);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26367, 0, 6, 32, 20, 3, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26373, 0, 6, 32, 20, 3, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26368, 0, 6, 32, 20, 3, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26374, 0, 6, 32, 20, 3, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
            switch (direction)
            {
                case 1:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 2:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x00793308 */
static void multi_dimension_rc_track_right_flyer_twist_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    switch (trackSequence)
    {
        case 0:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26377, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26383, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26382, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26388, 0, 6, 32, 20, 3, height + 24);
                    break;
            }

            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                0xFFFF, 0);
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);

            if (direction == 0 || direction == 3)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
            }
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 1:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26378, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26384, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26381, 0, 6, 32, 20, 3, height + 24);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26387, 0, 6, 32, 20, 3, height + 24);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
        case 2:
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26379, 0, 6, 32, 20, 3, height);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26385, 0, 6, 32, 20, 3, height);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26380, 0, 6, 32, 20, 3, height);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours[SCHEME_TRACK] | 26386, 0, 6, 32, 20, 3, height);
                    break;
            }
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
            switch (direction)
            {
                case 1:
                    paint_util_push_tunnel_right(session, height, TUNNEL_SQUARE_FLAT);
                    break;
                case 2:
                    paint_util_push_tunnel_left(session, height, TUNNEL_SQUARE_FLAT);
                    break;
            }
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
                0xFFFF, 0);
            paint_util_set_general_support_height(session, height + 32, 0x20);
            break;
    }
}

/** rct2: 0x00793398 */
static void multi_dimension_rc_track_multidim_inverted_flat_to_90_deg_quarter_loop_down(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26539, 0, 0, 32, 20, 3, height + 16, 0, 6,
                            height + 16);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26542, 0, 0, 2, 20, 3, height + 16, 0, 6,
                            height + 18);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26545, 0, 0, 2, 20, 3, height + 16, 0, 6,
                            height + 18);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26548, 0, 0, 32, 20, 3, height + 16, 0, 6,
                            height + 16);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height + 32, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 64, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26538, 0, 0, 2, 20, 31, height + 16, 24, 6,
                            height + 16);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26541, 0, 0, 2, 20, 3, height + 16, 0, 6,
                            height + 18);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26544, 0, 0, 2, 20, 3, height + 16, 0, 6,
                            height + 18);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26547, 0, 0, 2, 20, 31, height + 16, 24, 6,
                            height + 16);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 80, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26537, 0, 0, 2, 20, 31, height + 16, 28, 6,
                            height + 24);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26540, 0, 0, 2, 20, 31, height + 16, 0, 6,
                            height + 24);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26543, 0, 0, 2, 20, 31, height + 16, 0, 6,
                            height + 24);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26546, 0, 0, 2, 20, 31, height + 16, 29, 6,
                            height + 24);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 3:
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16208, 0, 0, 32, 20, 3, height, 0, 6,
                            height + 24);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16211, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16214, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16217, 0, 0, 32, 20, 3, height, 0, 6,
                            height + 24);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height + 16, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16207, 0, 0, 2, 20, 31, height, -8, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16210, 0, 0, 2, 20, 63, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16213, 0, 0, 2, 20, 63, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16216, 0, 0, 2, 20, 31, height, -8, 6,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 64, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16206, 0, 0, 2, 20, 31, height, 4, 6,
                            height + 8);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16209, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16212, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16215, 0, 0, 2, 20, 31, height, 4, 6,
                            height + 8);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 88, 0x20);
                break;
            case 3:
                break;
        }
    }
}

/** rct2: 0x00793318 */
static void multi_dimension_rc_track_block_brakes(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    bool isClosed = trackElement.BlockBrakeClosed();

    if (!trackElement.IsInverted())
    {
        static constexpr const uint32_t imageIds[NumOrthogonalDirections][2] = {
            { MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_SW_NE_OPEN, MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_SW_NE_CLOSED },
            { MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_NW_SE_OPEN, MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_NW_SE_CLOSED },
            { MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_SW_NE_OPEN, MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_SW_NE_CLOSED },
            { MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_NW_SE_OPEN, MULTI_DIM_NON_INVERTED_BLOCK_BRAKE_NW_SE_CLOSED },
        };

        PaintAddImageAsParentRotated(
            session, direction, session.TrackColours[SCHEME_TRACK] | imageIds[direction][isClosed], 0, 0, 32, 20, 3, height, 0,
            6, height);
        metal_a_supports_paint_setup(
            session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height, session.TrackColours[SCHEME_SUPPORTS]);
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
    else
    {
        static constexpr const uint32_t imageIds[NumOrthogonalDirections][2] = {
            { MULTI_DIM_INVERTED_BLOCK_BRAKE_SW_NE_OPEN, MULTI_DIM_INVERTED_BLOCK_BRAKE_SW_NE_CLOSED },
            { MULTI_DIM_INVERTED_BLOCK_BRAKE_NW_SE_OPEN, MULTI_DIM_INVERTED_BLOCK_BRAKE_NW_SE_CLOSED },
            { MULTI_DIM_INVERTED_BLOCK_BRAKE_SW_NE_OPEN, MULTI_DIM_INVERTED_BLOCK_BRAKE_SW_NE_CLOSED },
            { MULTI_DIM_INVERTED_BLOCK_BRAKE_NW_SE_OPEN, MULTI_DIM_INVERTED_BLOCK_BRAKE_NW_SE_CLOSED },
        };

        PaintAddImageAsParentRotated(
            session, direction, session.TrackColours[SCHEME_TRACK] | imageIds[direction][isClosed], 0, 0, 32, 20, 3,
            height + 24, 0, 6, height + 24);

        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        if (track_paint_util_should_paint_supports(session.MapPosition))
        {
            metal_a_supports_paint_setup(
                session, METAL_SUPPORTS_TUBES_INVERTED, 4, 0, height + 36, session.TrackColours[SCHEME_SUPPORTS]);
        }

        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
        paint_util_set_general_support_height(session, height + 32, 0x20);
    }
}

/** rct2: 0x00793388 */
static void multi_dimension_rc_track_multidim_90_deg_up_to_inverted_flat_quarter_loop(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16206, 0, 0, 2, 20, 31, height, 4, 6,
                            height + 8);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16209, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16212, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16215, 0, 0, 2, 20, 31, height, 4, 6,
                            height + 8);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 88, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16207, 0, 0, 2, 20, 31, height, -8, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16210, 0, 0, 2, 20, 63, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16213, 0, 0, 2, 20, 63, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16216, 0, 0, 2, 20, 31, height, -8, 6,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 64, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16208, 0, 0, 32, 20, 3, height, 0, 6,
                            height + 24);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16211, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16214, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16217, 0, 0, 32, 20, 3, height, 0, 6,
                            height + 24);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height + 16, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26537, 0, 0, 2, 20, 31, height - 16, 28, 6,
                            height - 8);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26540, 0, 0, 2, 20, 31, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26543, 0, 0, 2, 20, 31, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26546, 0, 0, 2, 20, 31, height - 16, 28, 6,
                            height - 8);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26538, 0, 0, 2, 20, 31, height - 16, 24, 6,
                            height - 16);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26541, 0, 0, 2, 20, 63, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26544, 0, 0, 2, 20, 63, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26547, 0, 0, 2, 20, 31, height - 16, 24, 6,
                            height - 16);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26539, 0, 0, 32, 20, 3, height - 16, 0, 6,
                            height - 16);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26542, 0, 0, 2, 20, 31, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26545, 0, 0, 2, 20, 31, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26548, 0, 0, 32, 20, 3, height - 16, 0, 6,
                            height - 16);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

/** rct2: 0x00793398 */
static void multi_dimension_rc_track_multidim_flat_to_90_deg_down_quarter_loop(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26539, 0, 0, 32, 20, 3, height + 16, 0, 6,
                            height + 16);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26542, 0, 0, 2, 20, 3, height + 16, 0, 6,
                            height + 18);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26545, 0, 0, 2, 20, 3, height + 16, 0, 6,
                            height + 18);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26548, 0, 0, 32, 20, 3, height + 16, 0, 6,
                            height + 16);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height + 32, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 64, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26538, 0, 0, 2, 20, 31, height + 16, 24, 6,
                            height + 16);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26541, 0, 0, 2, 20, 3, height + 16, 0, 6,
                            height + 18);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26544, 0, 0, 2, 20, 3, height + 16, 0, 6,
                            height + 18);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26547, 0, 0, 2, 20, 31, height + 16, 24, 6,
                            height + 16);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 80, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26537, 0, 0, 2, 20, 31, height + 16, 28, 6,
                            height + 24);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26540, 0, 0, 2, 20, 31, height + 16, 0, 6,
                            height + 24);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26543, 0, 0, 2, 20, 31, height + 16, 0, 6,
                            height + 24);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26546, 0, 0, 2, 20, 31, height + 16, 29, 6,
                            height + 24);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 104, 0x20);
                break;
            case 3:
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16208, 0, 0, 32, 20, 3, height, 0, 6,
                            height + 24);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16211, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16214, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16217, 0, 0, 32, 20, 3, height, 0, 6,
                            height + 24);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height + 16, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16207, 0, 0, 2, 20, 31, height, -8, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16210, 0, 0, 2, 20, 63, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16213, 0, 0, 2, 20, 63, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16216, 0, 0, 2, 20, 31, height, -8, 6,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 64, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16206, 0, 0, 2, 20, 31, height, 4, 6,
                            height + 8);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16209, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16212, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16215, 0, 0, 2, 20, 31, height, 4, 6,
                            height + 8);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 88, 0x20);
                break;
            case 3:
                break;
        }
    }
}

/** rct2: 0x00793388 */
static void multi_dimension_rc_track_multidim_inverted_90_deg_up_to_flat_quarter_loop(
    paint_session& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TrackElement& trackElement)
{
    if (!trackElement.IsInverted())
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16206, 0, 0, 2, 20, 31, height, 4, 6,
                            height + 8);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16209, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16212, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16215, 0, 0, 2, 20, 31, height, 4, 6,
                            height + 8);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 88, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16207, 0, 0, 2, 20, 31, height, -8, 6,
                            height);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16210, 0, 0, 2, 20, 63, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16213, 0, 0, 2, 20, 63, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16216, 0, 0, 2, 20, 31, height, -8, 6,
                            height);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 64, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16208, 0, 0, 32, 20, 3, height, 0, 6,
                            height + 24);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16211, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16214, 0, 0, 2, 20, 31, height, 24, 6,
                            height + 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 16217, 0, 0, 32, 20, 3, height, 0, 6,
                            height + 24);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height + 16, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
        }
    }
    else
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26537, 0, 0, 2, 20, 31, height - 16, 28, 6,
                            height - 8);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26540, 0, 0, 2, 20, 31, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26543, 0, 0, 2, 20, 31, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26546, 0, 0, 2, 20, 31, height - 16, 28, 6,
                            height - 8);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 72, 0x20);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26538, 0, 0, 2, 20, 31, height - 16, 24, 6,
                            height - 16);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26541, 0, 0, 2, 20, 63, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26544, 0, 0, 2, 20, 63, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26547, 0, 0, 2, 20, 31, height - 16, 24, 6,
                            height - 16);
                        break;
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 48, 0x20);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26539, 0, 0, 32, 20, 3, height - 16, 0, 6,
                            height - 16);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26542, 0, 0, 2, 20, 31, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26545, 0, 0, 2, 20, 31, height - 16, 0, 6,
                            height - 8);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours[SCHEME_TRACK] | 26548, 0, 0, 32, 20, 3, height - 16, 0, 6,
                            height - 16);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_SQUARE_FLAT);
                }
                paint_util_set_segment_support_height(
                    session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + 32, 0x20);
                break;
        }
    }
}

TRACK_PAINT_FUNCTION get_track_paint_function_multi_dimension_rc(int32_t trackType)
{
    switch (trackType)
    {
        case TrackElemType::Flat:
            return multi_dimension_rc_track_flat;
        case TrackElemType::EndStation:
        case TrackElemType::BeginStation:
        case TrackElemType::MiddleStation:
            return multi_dimension_rc_track_station;
        case TrackElemType::Up25:
            return multi_dimension_rc_track_25_deg_up;
        case TrackElemType::Up60:
            return multi_dimension_rc_track_60_deg_up;
        case TrackElemType::FlatToUp25:
            return multi_dimension_rc_track_flat_to_25_deg_up;
        case TrackElemType::Up25ToUp60:
            return multi_dimension_rc_track_25_deg_up_to_60_deg_up;
        case TrackElemType::Up60ToUp25:
            return multi_dimension_rc_track_60_deg_up_to_25_deg_up;
        case TrackElemType::Up25ToFlat:
            return multi_dimension_rc_track_25_deg_up_to_flat;
        case TrackElemType::Down25:
            return multi_dimension_rc_track_25_deg_down;
        case TrackElemType::Down60:
            return multi_dimension_rc_track_60_deg_down;
        case TrackElemType::FlatToDown25:
            return multi_dimension_rc_track_flat_to_25_deg_down;
        case TrackElemType::Down25ToDown60:
            return multi_dimension_rc_track_25_deg_down_to_60_deg_down;
        case TrackElemType::Down60ToDown25:
            return multi_dimension_rc_track_60_deg_down_to_25_deg_down;
        case TrackElemType::Down25ToFlat:
            return multi_dimension_rc_track_25_deg_down_to_flat;
        case TrackElemType::LeftQuarterTurn5Tiles:
            return multi_dimension_rc_track_left_quarter_turn_5;
        case TrackElemType::RightQuarterTurn5Tiles:
            return multi_dimension_rc_track_right_quarter_turn_5;
        case TrackElemType::FlatToLeftBank:
            return multi_dimension_rc_track_flat_to_left_bank;
        case TrackElemType::FlatToRightBank:
            return multi_dimension_rc_track_flat_to_right_bank;
        case TrackElemType::LeftBankToFlat:
            return multi_dimension_rc_track_left_bank_to_flat;
        case TrackElemType::RightBankToFlat:
            return multi_dimension_rc_track_right_bank_to_flat;
        case TrackElemType::BankedLeftQuarterTurn5Tiles:
            return multi_dimension_rc_track_banked_left_quarter_turn_5;
        case TrackElemType::BankedRightQuarterTurn5Tiles:
            return multi_dimension_rc_track_banked_right_quarter_turn_5;
        case TrackElemType::LeftBankToUp25:
            return multi_dimension_rc_track_left_bank_to_25_deg_up;
        case TrackElemType::RightBankToUp25:
            return multi_dimension_rc_track_right_bank_to_25_deg_up;
        case TrackElemType::Up25ToLeftBank:
            return multi_dimension_rc_track_25_deg_up_to_left_bank;
        case TrackElemType::Up25ToRightBank:
            return multi_dimension_rc_track_25_deg_up_to_right_bank;
        case TrackElemType::LeftBankToDown25:
            return multi_dimension_rc_track_left_bank_to_25_deg_down;
        case TrackElemType::RightBankToDown25:
            return multi_dimension_rc_track_right_bank_to_25_deg_down;
        case TrackElemType::Down25ToLeftBank:
            return multi_dimension_rc_track_25_deg_down_to_left_bank;
        case TrackElemType::Down25ToRightBank:
            return multi_dimension_rc_track_25_deg_down_to_right_bank;
        case TrackElemType::LeftBank:
            return multi_dimension_rc_track_left_bank;
        case TrackElemType::RightBank:
            return multi_dimension_rc_track_right_bank;
        case TrackElemType::SBendLeft:
            return multi_dimension_rc_track_s_bend_left;
        case TrackElemType::SBendRight:
            return multi_dimension_rc_track_s_bend_right;
        case TrackElemType::LeftQuarterTurn3Tiles:
            return multi_dimension_rc_track_left_quarter_turn_3;
        case TrackElemType::RightQuarterTurn3Tiles:
            return multi_dimension_rc_track_right_quarter_turn_3;
        case TrackElemType::LeftBankedQuarterTurn3Tiles:
            return multi_dimension_rc_track_left_quarter_turn_3_bank;
        case TrackElemType::RightBankedQuarterTurn3Tiles:
            return multi_dimension_rc_track_right_quarter_turn_3_bank;
        case TrackElemType::LeftHalfBankedHelixUpSmall:
            return multi_dimension_rc_track_left_half_banked_helix_up_small;
        case TrackElemType::RightHalfBankedHelixUpSmall:
            return multi_dimension_rc_track_right_half_banked_helix_up_small;
        case TrackElemType::LeftHalfBankedHelixDownSmall:
            return multi_dimension_rc_track_left_half_banked_helix_down_small;
        case TrackElemType::RightHalfBankedHelixDownSmall:
            return multi_dimension_rc_track_right_half_banked_helix_down_small;
        case TrackElemType::LeftHalfBankedHelixUpLarge:
            return multi_dimension_rc_track_left_half_banked_helix_up_large;
        case TrackElemType::RightHalfBankedHelixUpLarge:
            return multi_dimension_rc_track_right_half_banked_helix_up_large;
        case TrackElemType::LeftHalfBankedHelixDownLarge:
            return multi_dimension_rc_track_left_half_banked_helix_down_large;
        case TrackElemType::RightHalfBankedHelixDownLarge:
            return multi_dimension_rc_track_right_half_banked_helix_down_large;
        case TrackElemType::Brakes:
            return multi_dimension_rc_track_brakes;
        case TrackElemType::OnRidePhoto:
            return multi_dimension_rc_track_on_ride_photo;
        case TrackElemType::Up90:
            return multi_dimension_rc_track_90_deg_up;
        case TrackElemType::Down90:
            return multi_dimension_rc_track_90_deg_down;
        case TrackElemType::Up60ToUp90:
            return multi_dimension_rc_track_60_deg_up_to_90_deg_up;
        case TrackElemType::Down90ToDown60:
            return multi_dimension_rc_track_90_deg_down_to_60_deg_down;
        case TrackElemType::Up90ToUp60:
            return multi_dimension_rc_track_90_deg_up_to_60_deg_up;
        case TrackElemType::Down60ToDown90:
            return multi_dimension_rc_track_60_deg_down_to_90_deg_down;
        case TrackElemType::LeftEighthToDiag:
            return multi_dimension_rc_track_left_eighth_to_diag;
        case TrackElemType::RightEighthToDiag:
            return multi_dimension_rc_track_right_eighth_to_diag;
        case TrackElemType::LeftEighthToOrthogonal:
            return multi_dimension_rc_track_left_eighth_to_orthogonal;
        case TrackElemType::RightEighthToOrthogonal:
            return multi_dimension_rc_track_right_eighth_to_orthogonal;
        case TrackElemType::LeftEighthBankToDiag:
            return multi_dimension_rc_track_left_eighth_bank_to_diag;
        case TrackElemType::RightEighthBankToDiag:
            return multi_dimension_rc_track_right_eighth_bank_to_diag;
        case TrackElemType::LeftEighthBankToOrthogonal:
            return multi_dimension_rc_track_left_eighth_bank_to_orthogonal;
        case TrackElemType::RightEighthBankToOrthogonal:
            return multi_dimension_rc_track_right_eighth_bank_to_orthogonal;
        case TrackElemType::DiagFlat:
            return multi_dimension_rc_track_diag_flat;
        case TrackElemType::DiagUp25:
            return multi_dimension_rc_track_diag_25_deg_up;
        case TrackElemType::DiagUp60:
            return multi_dimension_rc_track_diag_60_deg_up;
        case TrackElemType::DiagFlatToUp25:
            return multi_dimension_rc_track_diag_flat_to_25_deg_up;
        case TrackElemType::DiagUp25ToUp60:
            return multi_dimension_rc_track_diag_25_deg_up_to_60_deg_up;
        case TrackElemType::DiagUp60ToUp25:
            return multi_dimension_rc_track_diag_60_deg_up_to_25_deg_up;
        case TrackElemType::DiagUp25ToFlat:
            return multi_dimension_rc_track_diag_25_deg_up_to_flat;
        case TrackElemType::DiagDown25:
            return multi_dimension_rc_track_diag_25_deg_down;
        case TrackElemType::DiagDown60:
            return multi_dimension_rc_track_diag_60_deg_down;
        case TrackElemType::DiagFlatToDown25:
            return multi_dimension_rc_track_diag_flat_to_25_deg_down;
        case TrackElemType::DiagDown25ToDown60:
            return multi_dimension_rc_track_diag_25_deg_down_to_60_deg_down;
        case TrackElemType::DiagDown60ToDown25:
            return multi_dimension_rc_track_diag_60_deg_down_to_25_deg_down;
        case TrackElemType::DiagDown25ToFlat:
            return multi_dimension_rc_track_diag_25_deg_down_to_flat;
        case TrackElemType::DiagFlatToLeftBank:
            return multi_dimension_rc_track_diag_flat_to_left_bank;
        case TrackElemType::DiagFlatToRightBank:
            return multi_dimension_rc_track_diag_flat_to_right_bank;
        case TrackElemType::DiagLeftBankToFlat:
            return multi_dimension_rc_track_diag_left_bank_to_flat;
        case TrackElemType::DiagRightBankToFlat:
            return multi_dimension_rc_track_diag_right_bank_to_flat;
        case TrackElemType::DiagLeftBankToUp25:
            return multi_dimension_rc_track_diag_left_bank_to_25_deg_up;
        case TrackElemType::DiagRightBankToUp25:
            return multi_dimension_rc_track_diag_right_bank_to_25_deg_up;
        case TrackElemType::DiagUp25ToLeftBank:
            return multi_dimension_rc_track_diag_25_deg_up_to_left_bank;
        case TrackElemType::DiagUp25ToRightBank:
            return multi_dimension_rc_track_diag_25_deg_up_to_right_bank;
        case TrackElemType::DiagLeftBankToDown25:
            return multi_dimension_rc_track_diag_left_bank_to_25_deg_down;
        case TrackElemType::DiagRightBankToDown25:
            return multi_dimension_rc_track_diag_right_bank_to_25_deg_down;
        case TrackElemType::DiagDown25ToLeftBank:
            return multi_dimension_rc_track_diag_25_deg_down_to_left_bank;
        case TrackElemType::DiagDown25ToRightBank:
            return multi_dimension_rc_track_diag_25_deg_down_to_right_bank;
        case TrackElemType::DiagLeftBank:
            return multi_dimension_rc_track_diag_left_bank;
        case TrackElemType::DiagRightBank:
            return multi_dimension_rc_track_diag_right_bank;
        case TrackElemType::LeftFlyerTwistUp:
            return multi_dimension_rc_track_left_flyer_twist_up;
        case TrackElemType::RightFlyerTwistUp:
            return multi_dimension_rc_track_right_flyer_twist_up;
        case TrackElemType::LeftFlyerTwistDown:
            return multi_dimension_rc_track_left_flyer_twist_down;
        case TrackElemType::RightFlyerTwistDown:
            return multi_dimension_rc_track_right_flyer_twist_down;
        case TrackElemType::MultiDimInvertedFlatToDown90QuarterLoop:
            return multi_dimension_rc_track_multidim_inverted_flat_to_90_deg_quarter_loop_down;
        case TrackElemType::BlockBrakes:
            return multi_dimension_rc_track_block_brakes;
        case TrackElemType::MultiDimUp90ToInvertedFlatQuarterLoop:
            return multi_dimension_rc_track_multidim_90_deg_up_to_inverted_flat_quarter_loop;
        case TrackElemType::MultiDimFlatToDown90QuarterLoop:
            return multi_dimension_rc_track_multidim_flat_to_90_deg_down_quarter_loop;
        case TrackElemType::MultiDimInvertedUp90ToFlatQuarterLoop:
            return multi_dimension_rc_track_multidim_inverted_90_deg_up_to_flat_quarter_loop;
    }
    return nullptr;
}
