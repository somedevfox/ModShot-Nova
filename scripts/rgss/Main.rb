#==============================================================================
# ** Main
#------------------------------------------------------------------------------
#  After defining each class, actual processing begins here.
#==============================================================================

at_exit do
  Wallpaper.reset
  save unless $game_switches[99] || ($game_system.map_interpreter.running? || !$scene.is_a?(Scene_Map))
end

module Audio
  class << self
    def method_missing(*args)
    end
  end
end

result, bank = FMOD::Studio::System.load_bank_file("Master.bank", FMOD::Studio::LoadBankFlags::NONBLOCKING)
res2, strbank = FMOD::Studio::System.load_bank_file("Master.strings.bank", 0)
puts bank.is_valid.to_s
puts bank.get_loading_state.to_s
puts bank.get_path.to_s
#strbank.get_string_count[1].times do |i|
#  puts strbank.get_string_info(i).to_s
#end

puts bank.get_user_data
puts bank.set_user_data(Game_Map.new)
puts bank.get_user_data.inspect

begin
  $console = Graphics.fullscreen
  Graphics.frame_rate = 60
  Font.default_size = 20

  # Load persistent data
  Persistent.load

  # Prepare for transition
  Graphics.freeze
  $demo = false
  $GDC = false
  # Make scene object (title screen)
  $scene = Scene_Title.new
  Oneshot.allow_exit false
  Oneshot.exiting false

  #  x = Oneshot.textinput("Foo Bar")
  #  print("#{x}")

  # Call main method as long as $scene is effective
  while $scene != nil
    $scene.main
  end
  # Fade out
  Oneshot.exiting true
  Graphics.transition(20)

  if Journal.active?
    Journal.set ""
  end

  Oneshot.allow_exit true
rescue Errno::ENOENT
  # Supplement Errno::ENOENT exception
  # If unable to open file, display message and end
  filename = $!.message.sub("No such file or directory - ", "")
  print("Unable to find file #{filename}.")
end
