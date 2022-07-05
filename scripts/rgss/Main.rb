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

puts FMOD::Studio::System.initialize(512, 0, 0)
puts FMOD::Studio::System.is_valid

result, bank = FMOD::Studio::System.load_bank_file("Master.bank", FMOD::Studio::LoadBankFlags::NONBLOCKING)
res2, strbank = FMOD::Studio::System.load_bank_file("Master.strings.bank", 0)
puts bank.is_valid.to_s
puts bank.get_path.to_s

result, list = bank.get_vca_list
puts list.to_s
list.each_with_index do |v, i|
  puts "index: #{i} vca: #{v} valid: #{v.is_valid}"
  puts v.get_id.to_s
  puts v.get_path.to_s
  puts v.get_volume.to_s
end

result, list2 = bank.get_bus_list
puts list2.to_s
list2.each_with_index do |b, i|
  puts "index: #{i} bus: #{b} valid: #{b.is_valid}"
  puts b.get_id.to_s
  puts b.get_path.to_s
  puts b.get_volume.to_s
  puts b.get_paused.to_s
  puts b.get_port_index.to_s
  puts b.get_cpu_usage.to_s
  puts b.get_memory_usage.to_s
end

result, list3 = bank.get_event_list
puts list3.to_s
list3.each_with_index do |b, i|
  puts "index: #{i} bus: #{b} valid: #{b.is_valid}"
  puts b.get_id.to_s
  puts b.get_path.to_s
end

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
