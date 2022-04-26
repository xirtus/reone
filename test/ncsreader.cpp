/*
 * Copyright (c) 2020-2021 The reone project contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <boost/test/unit_test.hpp>

#include "../src/script/format/ncsreader.h"
#include "../src/script/program.h"

using namespace std;

using namespace reone;
using namespace reone::script;

BOOST_AUTO_TEST_SUITE(ncs_reader)

BOOST_AUTO_TEST_CASE(should_read_ncs) {
    // given

    auto ss = ostringstream();
    ss << "NCS V1.0";
    ss << string("\x42\x00\x00\x01\x2c", 5);                      // T
    ss << string("\x01\x01\xff\xff\xff\xfc\x00\x04", 8);          // CPDOWNSP
    ss << string("\x03\x01\xff\xff\xff\xfc\x00\x04", 8);          // CPTOPSP
    ss << string("\x26\x01\xff\xff\xff\xfc\x00\x04", 8);          // CPDOWNBP
    ss << string("\x27\x01\xff\xff\xff\xfc\x00\x04", 8);          // CPTOPBP
    ss << string("\x04\x03\x00\x00\x00\x01", 6);                  // CONSTI
    ss << string("\x04\x04\x3f\x80\x00\x00", 6);                  // CONSTF
    ss << string("\x04\x05\x00\x02\x41\x61", 6);                  // CONSTS
    ss << string("\x04\x06\x00\x00\x00\x02", 6);                  // CONSTO
    ss << string("\x05\x00\x00\x01\x02", 5);                      // ACTION
    ss << string("\x1b\x00\xff\xff\xff\xfc", 6);                  // MOVSP
    ss << string("\x1d\x00\x00\x00\x00\x01", 6);                  // JMP
    ss << string("\x1e\x00\x00\x00\x00\x02", 6);                  // JSR
    ss << string("\x1f\x00\x00\x00\x00\x03", 6);                  // JZ
    ss << string("\x25\x00\x00\x00\x00\x04", 6);                  // JNZ
    ss << string("\x21\x01\x00\x08\xff\xfc\x00\x04", 8);          // DESTRUCT
    ss << string("\x23\x03\xff\xff\xff\xfc", 6);                  // DECISP
    ss << string("\x24\x03\xff\xff\xff\xfc", 6);                  // INCISP
    ss << string("\x28\x03\xff\xff\xff\xfc", 6);                  // DECIBP
    ss << string("\x29\x03\xff\xff\xff\xfc", 6);                  // INCIBP
    ss << string("\x2c\x10\x00\x00\x00\x04\x00\x00\x00\x08", 10); // STORE_STATE
    ss << string("\x0b\x24\x00\x04", 4);                          // EQUALTT
    ss << string("\x0c\x24\x00\x04", 4);                          // NEQUALTT
    ss << string("\x00\x0c", 2);                                  // NOP
    ss << string("\x02\x03", 2);                                  // RSADDI
    ss << string("\x02\x04", 2);                                  // RSADDF
    ss << string("\x02\x05", 2);                                  // RSADDS
    ss << string("\x02\x06", 2);                                  // RSADDO
    ss << string("\x02\x10", 2);                                  // RSADDEFF
    ss << string("\x02\x11", 2);                                  // RSADDEVT
    ss << string("\x02\x12", 2);                                  // RSADDLOC
    ss << string("\x02\x13", 2);                                  // RSADDTAL
    ss << string("\x06\x20", 2);                                  // LOGANDII
    ss << string("\x07\x20", 2);                                  // LOGORII
    ss << string("\x08\x20", 2);                                  // INCORII
    ss << string("\x09\x20", 2);                                  // EXCORII
    ss << string("\x0a\x20", 2);                                  // BOOLANDII
    ss << string("\x0b\x20", 2);                                  // EQUALII
    ss << string("\x0b\x21", 2);                                  // EQUALFF
    ss << string("\x0b\x23", 2);                                  // EQUALSS
    ss << string("\x0b\x22", 2);                                  // EQUALOO
    ss << string("\x0b\x30", 2);                                  // EQUALEFFEFF
    ss << string("\x0b\x31", 2);                                  // EQUALEVTEVT
    ss << string("\x0b\x32", 2);                                  // EQUALLOCLOC
    ss << string("\x0b\x33", 2);                                  // EQUALTALTAL
    ss << string("\x0c\x20", 2);                                  // NEQUALII
    ss << string("\x0c\x21", 2);                                  // NEQUALFF
    ss << string("\x0c\x23", 2);                                  // NEQUALSS
    ss << string("\x0c\x22", 2);                                  // NEQUALOO
    ss << string("\x0c\x30", 2);                                  // NEQUALEFFEFF
    ss << string("\x0c\x31", 2);                                  // NEQUALEVTEVT
    ss << string("\x0c\x32", 2);                                  // NEQUALLOCLOC
    ss << string("\x0c\x33", 2);                                  // NEQUALTALTAL
    ss << string("\x0d\x20", 2);                                  // GEQII
    ss << string("\x0d\x21", 2);                                  // GEQFF
    ss << string("\x0e\x20", 2);                                  // GTII
    ss << string("\x0e\x21", 2);                                  // GTFF
    ss << string("\x0f\x20", 2);                                  // LTII
    ss << string("\x0f\x21", 2);                                  // LTFF
    ss << string("\x10\x20", 2);                                  // LEQII
    ss << string("\x10\x21", 2);                                  // LEQFF
    ss << string("\x11\x20", 2);                                  // SHLEFTII
    ss << string("\x12\x20", 2);                                  // SHRIGHTII
    ss << string("\x13\x20", 2);                                  // USHRIGHTII
    ss << string("\x14\x20", 2);                                  // ADDII
    ss << string("\x14\x25", 2);                                  // ADDIF
    ss << string("\x14\x26", 2);                                  // ADDFI
    ss << string("\x14\x21", 2);                                  // ADDFF
    ss << string("\x14\x23", 2);                                  // ADDSS
    ss << string("\x14\x3a", 2);                                  // ADDVV
    ss << string("\x15\x20", 2);                                  // SUBII
    ss << string("\x15\x25", 2);                                  // SUBIF
    ss << string("\x15\x26", 2);                                  // SUBFI
    ss << string("\x15\x21", 2);                                  // SUBFF
    ss << string("\x15\x3a", 2);                                  // SUBVV
    ss << string("\x16\x20", 2);                                  // MULII
    ss << string("\x16\x25", 2);                                  // MULIF
    ss << string("\x16\x26", 2);                                  // MULFI
    ss << string("\x16\x21", 2);                                  // MULFF
    ss << string("\x16\x3b", 2);                                  // MULVF
    ss << string("\x16\x3c", 2);                                  // MULFV
    ss << string("\x17\x20", 2);                                  // DIVII
    ss << string("\x17\x25", 2);                                  // DIVIF
    ss << string("\x17\x26", 2);                                  // DIVFI
    ss << string("\x17\x21", 2);                                  // DIVFF
    ss << string("\x17\x3b", 2);                                  // DIVVF
    ss << string("\x17\x3c", 2);                                  // DIVFV
    ss << string("\x18\x20", 2);                                  // MODII
    ss << string("\x19\x03", 2);                                  // NEGI
    ss << string("\x19\x04", 2);                                  // NEGF
    ss << string("\x1a\x03", 2);                                  // COMPI
    ss << string("\x20\x00", 2);                                  // RETN
    ss << string("\x22\x03", 2);                                  // NOTI
    ss << string("\x2a\x00", 2);                                  // SAVEBP
    ss << string("\x2b\x00", 2);                                  // RESTOREBP
    ss << string("\x2d\x00", 2);                                  // NOP2

    auto reader = NcsReader("");
    auto stream = make_shared<istringstream>(ss.str());

    // when

    reader.load(stream);

    // then

    auto program = reader.program();
    BOOST_CHECK_EQUAL(300, program->length());
    BOOST_CHECK_EQUAL(95ll, program->instructions().size());
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::CPDOWNSP), static_cast<int>(program->getInstruction(13).type));
    BOOST_CHECK_EQUAL(-4, program->getInstruction(13).stackOffset);
    BOOST_CHECK_EQUAL(4, program->getInstruction(13).size);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::CPTOPSP), static_cast<int>(program->getInstruction(21).type));
    BOOST_CHECK_EQUAL(-4, program->getInstruction(21).stackOffset);
    BOOST_CHECK_EQUAL(4, program->getInstruction(21).size);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::CPDOWNBP), static_cast<int>(program->getInstruction(29).type));
    BOOST_CHECK_EQUAL(-4, program->getInstruction(29).stackOffset);
    BOOST_CHECK_EQUAL(4, program->getInstruction(29).size);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::CPTOPBP), static_cast<int>(program->getInstruction(37).type));
    BOOST_CHECK_EQUAL(-4, program->getInstruction(37).stackOffset);
    BOOST_CHECK_EQUAL(4, program->getInstruction(37).size);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::CONSTI), static_cast<int>(program->getInstruction(45).type));
    BOOST_CHECK_EQUAL(1, program->getInstruction(45).intValue);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::CONSTF), static_cast<int>(program->getInstruction(51).type));
    BOOST_CHECK_EQUAL(1.0f, program->getInstruction(51).floatValue);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::CONSTS), static_cast<int>(program->getInstruction(57).type));
    BOOST_CHECK_EQUAL("Aa", program->getInstruction(57).strValue);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::CONSTO), static_cast<int>(program->getInstruction(63).type));
    BOOST_CHECK_EQUAL(2, program->getInstruction(63).objectId);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::ACTION), static_cast<int>(program->getInstruction(69).type));
    BOOST_CHECK_EQUAL(1, program->getInstruction(69).routine);
    BOOST_CHECK_EQUAL(2, program->getInstruction(69).argCount);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::MOVSP), static_cast<int>(program->getInstruction(74).type));
    BOOST_CHECK_EQUAL(-4, program->getInstruction(74).stackOffset);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::JMP), static_cast<int>(program->getInstruction(80).type));
    BOOST_CHECK_EQUAL(1, program->getInstruction(80).jumpOffset);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::JSR), static_cast<int>(program->getInstruction(86).type));
    BOOST_CHECK_EQUAL(2, program->getInstruction(86).jumpOffset);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::JZ), static_cast<int>(program->getInstruction(92).type));
    BOOST_CHECK_EQUAL(3, program->getInstruction(92).jumpOffset);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::JNZ), static_cast<int>(program->getInstruction(98).type));
    BOOST_CHECK_EQUAL(4, program->getInstruction(98).jumpOffset);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::DESTRUCT), static_cast<int>(program->getInstruction(104).type));
    BOOST_CHECK_EQUAL(8, program->getInstruction(104).size);
    BOOST_CHECK_EQUAL(-4, program->getInstruction(104).stackOffset);
    BOOST_CHECK_EQUAL(4, program->getInstruction(104).sizeNoDestroy);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::DECISP), static_cast<int>(program->getInstruction(112).type));
    BOOST_CHECK_EQUAL(-4, program->getInstruction(112).stackOffset);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::INCISP), static_cast<int>(program->getInstruction(118).type));
    BOOST_CHECK_EQUAL(-4, program->getInstruction(118).stackOffset);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::DECIBP), static_cast<int>(program->getInstruction(124).type));
    BOOST_CHECK_EQUAL(-4, program->getInstruction(124).stackOffset);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::INCIBP), static_cast<int>(program->getInstruction(130).type));
    BOOST_CHECK_EQUAL(-4, program->getInstruction(130).stackOffset);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::STORE_STATE), static_cast<int>(program->getInstruction(136).type));
    BOOST_CHECK_EQUAL(4, program->getInstruction(136).size);
    BOOST_CHECK_EQUAL(8, program->getInstruction(136).sizeLocals);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::EQUALTT), static_cast<int>(program->getInstruction(146).type));
    BOOST_CHECK_EQUAL(4, program->getInstruction(146).size);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NEQUALTT), static_cast<int>(program->getInstruction(150).type));
    BOOST_CHECK_EQUAL(4, program->getInstruction(150).size);
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NOP), static_cast<int>(program->getInstruction(154).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::RSADDI), static_cast<int>(program->getInstruction(156).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::RSADDF), static_cast<int>(program->getInstruction(158).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::RSADDS), static_cast<int>(program->getInstruction(160).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::RSADDO), static_cast<int>(program->getInstruction(162).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::RSADDEFF), static_cast<int>(program->getInstruction(164).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::RSADDEVT), static_cast<int>(program->getInstruction(166).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::RSADDLOC), static_cast<int>(program->getInstruction(168).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::RSADDTAL), static_cast<int>(program->getInstruction(170).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::LOGANDII), static_cast<int>(program->getInstruction(172).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::LOGORII), static_cast<int>(program->getInstruction(174).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::INCORII), static_cast<int>(program->getInstruction(176).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::EXCORII), static_cast<int>(program->getInstruction(178).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::BOOLANDII), static_cast<int>(program->getInstruction(180).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::EQUALII), static_cast<int>(program->getInstruction(182).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::EQUALFF), static_cast<int>(program->getInstruction(184).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::EQUALSS), static_cast<int>(program->getInstruction(186).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::EQUALOO), static_cast<int>(program->getInstruction(188).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::EQUALEFFEFF), static_cast<int>(program->getInstruction(190).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::EQUALEVTEVT), static_cast<int>(program->getInstruction(192).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::EQUALLOCLOC), static_cast<int>(program->getInstruction(194).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::EQUALTALTAL), static_cast<int>(program->getInstruction(196).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NEQUALII), static_cast<int>(program->getInstruction(198).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NEQUALFF), static_cast<int>(program->getInstruction(200).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NEQUALSS), static_cast<int>(program->getInstruction(202).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NEQUALOO), static_cast<int>(program->getInstruction(204).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NEQUALEFFEFF), static_cast<int>(program->getInstruction(206).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NEQUALEVTEVT), static_cast<int>(program->getInstruction(208).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NEQUALLOCLOC), static_cast<int>(program->getInstruction(210).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NEQUALTALTAL), static_cast<int>(program->getInstruction(212).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::GEQII), static_cast<int>(program->getInstruction(214).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::GEQFF), static_cast<int>(program->getInstruction(216).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::GTII), static_cast<int>(program->getInstruction(218).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::GTFF), static_cast<int>(program->getInstruction(220).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::LTII), static_cast<int>(program->getInstruction(222).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::LTFF), static_cast<int>(program->getInstruction(224).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::LEQII), static_cast<int>(program->getInstruction(226).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::LEQFF), static_cast<int>(program->getInstruction(228).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::SHLEFTII), static_cast<int>(program->getInstruction(230).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::SHRIGHTII), static_cast<int>(program->getInstruction(232).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::USHRIGHTII), static_cast<int>(program->getInstruction(234).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::ADDII), static_cast<int>(program->getInstruction(236).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::ADDIF), static_cast<int>(program->getInstruction(238).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::ADDFI), static_cast<int>(program->getInstruction(240).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::ADDFF), static_cast<int>(program->getInstruction(242).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::ADDSS), static_cast<int>(program->getInstruction(244).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::ADDVV), static_cast<int>(program->getInstruction(246).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::SUBII), static_cast<int>(program->getInstruction(248).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::SUBIF), static_cast<int>(program->getInstruction(250).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::SUBFI), static_cast<int>(program->getInstruction(252).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::SUBFF), static_cast<int>(program->getInstruction(254).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::SUBVV), static_cast<int>(program->getInstruction(256).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::MULII), static_cast<int>(program->getInstruction(258).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::MULIF), static_cast<int>(program->getInstruction(260).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::MULFI), static_cast<int>(program->getInstruction(262).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::MULFF), static_cast<int>(program->getInstruction(264).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::MULVF), static_cast<int>(program->getInstruction(266).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::MULFV), static_cast<int>(program->getInstruction(268).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::DIVII), static_cast<int>(program->getInstruction(270).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::DIVIF), static_cast<int>(program->getInstruction(272).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::DIVFI), static_cast<int>(program->getInstruction(274).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::DIVFF), static_cast<int>(program->getInstruction(276).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::DIVVF), static_cast<int>(program->getInstruction(278).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::DIVFV), static_cast<int>(program->getInstruction(280).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::MODII), static_cast<int>(program->getInstruction(282).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NEGI), static_cast<int>(program->getInstruction(284).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NEGF), static_cast<int>(program->getInstruction(286).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::COMPI), static_cast<int>(program->getInstruction(288).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::RETN), static_cast<int>(program->getInstruction(290).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NOTI), static_cast<int>(program->getInstruction(292).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::SAVEBP), static_cast<int>(program->getInstruction(294).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::RESTOREBP), static_cast<int>(program->getInstruction(296).type));
    BOOST_CHECK_EQUAL(static_cast<int>(InstructionType::NOP2), static_cast<int>(program->getInstruction(298).type));
}

BOOST_AUTO_TEST_SUITE_END()